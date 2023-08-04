// Header
#include "can_database.h"

// C++ Standard Libraries
#include <iostream>
#include <iomanip>

// C Standard Libraries
#include <string.h>

namespace Network
{
    CanDatabase::CanDatabase(const std::string& databaseFilePath, const std::string& canDeviceName) : txInterface(canDeviceName.c_str()), rxInterface(canDeviceName.c_str())
    {
        #ifdef DEBUG_MODE
        std::cout << "Creating CAN database bound to device \"" << canDeviceName << "\" using DBC file \"" << databaseFilePath << "\"..." << std::endl;
        #endif

        CanDbc::parseFile(databaseFilePath, &messages, &signals, &messageCount, &signalCount);

        #ifdef DEBUG_MODE
        std::cout << "Read DBC file. Allocating " << signalCount << " entries... " << std::endl;
        #endif

        this->allocate(signalCount);

        for(size_t index = 0; index < signalCount; ++index)
        {
            CanSignal& signal = signals[index];

            if(signal.datatypeId == ID_DATATYPE_UINT)
            {
                unsigned int data = 0;
                this->insert(data, signal.name);
            }
            else if(signal.datatypeId == ID_DATATYPE_INT)
            {
                int data = 0;
                this->insert(data, signal.name);
            }
            else if(signal.datatypeId == ID_DATATYPE_BOOL)
            {
                bool data = false;
                this->insert(data, signal.name);
            }
            else if(signal.datatypeId == ID_DATATYPE_DOUBLE)
            {
                double data = 0.0;
                this->insert(data, signal.name);
            }
            else
            {
                throw std::runtime_error("Failed to create CAN database: Signal \"" + std::string(signal.name) + "\" has unknown datatype.");
            }
        }

        rxInterface.setTimeoutMs(SOCKET_RX_TIMEOUT_MS);

        rxThreadControl = false;
        rxThreadDebug = false;

        this->startRxThread();
    }

    CanDatabase::~CanDatabase()
    {
        std::cout << "Terminating RX thread..." << std::endl;

        this->endRxThread();

        std::cout << "RX thread ended." << std::endl;

        // Delete allocated strings
        for(size_t index = 0; index < messageCount; ++index)
        {
            delete [] messages[index].name;
        }
        for(size_t index = 0; index < signalCount; ++index)
        {
            delete [] signals[index].name;
        }

        // Delete message and signal arrays
        delete [] messages;
        delete [] signals;
    }

    void* CanDatabase::scanRx(void* database_)
    {
        #ifdef DEBUG_MODE
        std::cout << "Beginning RX thread..." << std::endl;
        #endif
        
        CanDatabase* database = reinterpret_cast<CanDatabase*>(database_);

        uint16_t id;
        uint64_t data;
        uint8_t  dataLength;

        while(database->rxThreadControl)
        {
            #ifdef DEBUG_MODE
            if(database->rxThreadDebug) std::cout << "RX thread scanning..." << std::endl;
            #endif
            
            try
            {
                database->rxInterface.readMessage(&data, &dataLength, &id);

                bool messageFound = false;

                for(size_t messageIndex = 0; messageIndex < database->messageCount; ++messageIndex)
                {
                    if(id != database->messages[messageIndex].id) continue;
                    
                    CanMessage& message = database->messages[messageIndex];

                    for(size_t signalIndex = message.signalIndex; signalIndex < message.signalIndex + message.signalCount; ++signalIndex)
                    {
                        CanSignal& signal = database->signals[signalIndex];

                        if(signal.datatypeId == ID_DATATYPE_UINT)
                        {
                            unsigned int signalData = static_cast<unsigned int>(CanSocket::parseUnsignedInt(data, signal));
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_INT)
                        {
                            int signalData = static_cast<int>(CanSocket::parseSignedInt(data, signal));
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_BOOL)
                        {
                            bool signalData = CanSocket::parseBool(data, signal);
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_DOUBLE)
                        {
                            double signalData = CanSocket::parseDouble(data, signal);
                            database->set(signalIndex, signalData);
                        }
                    }

                    messageFound = true;
                }

                #ifdef DEBUG_MODE
                if(!messageFound && database->rxThreadDebug) std::cout << "Received unknown message, ID: " << std::hex << id << ". Ignoring..." << std::endl;
                #endif
            }
            catch(const PosixSocket::Timeout& exception) {}
            catch(const std::exception& exception)
            {
                std::cerr << "RX thread error: " << exception.what() << std::endl;
            }
        }

        return NULL;
    }

    void CanDatabase::startRxThread()
    {
        if(this->rxThreadControl) return;

        this->rxThreadControl = true;
        pthread_create(&rxThread, NULL, &CanDatabase::scanRx, this); // TODO: This function can fail. need to validate results
    }

    void CanDatabase::endRxThread()
    {
        if(!this->rxThreadControl) return;

        rxThreadControl = false;
        pthread_join(rxThread, NULL);
    }

    bool CanDatabase::getRxThreadStatus() const
    {
        return this->rxThreadControl;
    }

    void CanDatabase::setRxThreadDebug(bool state)
    {
        this->rxThreadDebug = state;
    }

    bool CanDatabase::getRxThreadDebug() const
    {
        return this->rxThreadDebug;
    }

    void CanDatabase::print(std::ostream& stream)
    {
        stream << "   " << std::setw(32) << "Signal Name";
        stream << " | " << std::setw(10) << "Value";
        stream << " | " << std::setw(8)  << "Bit Mask";
        stream << " | " << std::setw(10) << "Bit Length";
        stream << " | " << std::setw(12) << "Bit Position";
        stream << " | " << std::setw(4)  << "Type" << std::endl;
        
        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            const CanMessage& m = messages[mIndex];

            stream << m.name << " - ID: " << std::hex << std::setw(3) << m.id << " " << std::setw(84 - strlen(m.name)) << std::setfill('-') << "-" << std::endl;
            stream << std::setfill(' ');

            for(size_t sIndex = m.signalIndex; sIndex < m.signalIndex + m.signalCount; ++sIndex)
            {
                const CanSignal& s = m.signalArray[sIndex];

                stream << "   " << std::setw(32) << s.name;

                if(s.datatypeId == ID_DATATYPE_UINT)
                {
                    unsigned int data;
                    this->get(sIndex, data);
                    stream << " | " << std::setw(10) << std::dec << data;
                }
                else if(s.datatypeId == ID_DATATYPE_INT)
                {
                    int data;
                    this->get(sIndex, data);
                    stream << " | " << std::setw(10) << std::dec << data;
                }
                else if(s.datatypeId == ID_DATATYPE_BOOL)
                {
                    bool data;
                    this->get(sIndex, data);
                    stream << " | " << std::setw(10) << std::dec << data;
                }
                else if(s.datatypeId == ID_DATATYPE_DOUBLE)
                {
                    double data;
                    this->get(sIndex, data);
                    stream << " | " << std::setw(10) << std::dec << data;
                }
                else
                {
                    stream << " | " << std::setw(10) << "--";
                }

                stream << " | " << std::setw(8)  << std::hex << s.bitMask;
                stream << " | " << std::setw(10) << std::dec << s.bitLength;
                stream << " | " << std::setw(12) << std::dec << s.bitPosition;
                stream << " | " << std::setw(4)  << std::dec << s.datatypeId << std::endl;
            }

            stream << std::endl;
        }
    }
}