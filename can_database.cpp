// Header
#include "can_database.h"

// C++ Standard Libraries
#include <iostream>
#include <iomanip>

// POSIX Libraries
#include <string.h>

namespace Network
{
    CanDatabase::CanDatabase(std::string databaseFilePath, std::string canDeviceName) : txInterface(canDeviceName.c_str()), rxInterface(canDeviceName.c_str())
    {
        CanDbc::parseFile(databaseFilePath, &messages, &signals, &messageCount, &signalCount);

        this->allocate(signalCount);

        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            CanMessage& message = messages[mIndex];

            for(size_t sIndex = message.signalIndex; sIndex < message.signalIndex + message.signalCount; ++sIndex)
            {
                CanSignal& signal = signals[sIndex];

                // TODO: Need to figure out datatypes. Things are weird

                // if(s.typeId == ID_TYPE_INT)
                // {
                //     int data = 0;
                //     this->insert(data, s.name.c_str());
                //     signalIndices[mIndex][sIndex] = this->find(s.name.c_str());
                // }
                // else if(s.typeId == ID_TYPE_UINT)
                // {
                //     unsigned int data = 0;
                //     this->insert(data, s.name.c_str());
                //     signalIndices[mIndex][sIndex] = this->find(s.name.c_str());
                // }
                // else if(s.typeId == ID_TYPE_BOOL)
                // {
                //     bool data = false;
                //     this->insert(data, s.name.c_str());
                //     signalIndices[mIndex][sIndex] = this->find(s.name.c_str());
                // }
            }
        }

        rxInterface.setTimeoutMs(SOCKET_RX_TIMEOUT_MS);

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

        unsigned int id;
        uint64_t data;
        unsigned int dataCount;

        while(database->rxThreadControl)
        {
            #ifdef DEBUG_MODE
            if(database->rxThreadDebug) std::cout << "RX thread scanning..." << std::endl;
            #endif
            
            try
            {
                database->rxInterface.read_m(id, data, dataCount);

                bool messageFound = false;

                for(size_t mIndex = 0; mIndex < database->messageCount; ++mIndex)
                {
                    if(id != database->messages[mIndex].id) continue;
                    
                    CanMessage& message = database->messages[mIndex];

                    size_t signalCount = message.signals.size();
                    for(size_t sIndex = 0; sIndex < signalCount; ++sIndex)
                    {
                        CanSignal& signal = message.signals[sIndex];

                        uint64_t value = (data >> signal.bitPosition) & signal.bitMask;

                        if(signal.typeId == ID_TYPE_INT)
                        {
                            int data = *reinterpret_cast<int*>(&value);
                            database->set(database->signalIndices[mIndex][sIndex], data);
                        }
                        else if(signal.typeId == ID_TYPE_UINT)
                        {
                            unsigned int data = *reinterpret_cast<unsigned int*>(&value);
                            database->set(database->signalIndices[mIndex][sIndex], data);
                        }
                        else if(signal.typeId == ID_TYPE_BOOL)
                        {
                            bool data = *reinterpret_cast<bool*>(&value);
                            database->set(database->signalIndices[mIndex][sIndex], data);
                        }
                    }

                    messageFound = true;
                }

                #ifdef DEBUG_MODE
                if(!messageFound && database->rxThreadDebug) std::cout << "Received unknown message, ID: " << std::hex << id << ". Ignoring..." << std::endl;
                #endif
            }
            catch(const CanSocket::Timeout& exception) {}
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
        pthread_create(&rxThread, NULL, &CanDatabase::scanRx, this);
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

    const std::vector<CanMessage>& CanDatabase::getMessages() const
    {
        return this->messages;
    }

    void CanDatabase::printMessages(std::ostream& stream)
    {
        CanDatabase::printMessages(stream, messages, messageCount);
    }

    // TODO: Find a better place for this function
    void CanDatabase::printMessages(std::ostream& stream, CanMessage* messages, size_t messageCount)
    {
        stream << "   " << std::setw(32) << "Signal Name";
        stream << " | " << std::setw(8)  << "Bit Mask";
        stream << " | " << std::setw(10) << "Bit Length";
        stream << " | " << std::setw(12) << "Bit Position" << std::endl;
        
        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            const CanMessage& m = messages[mIndex];

            stream << m.name << " - ID: " << std::hex << std::setw(3) << m.id << " " << std::setw(64 - strlen(m.name)) << std::setfill('-') << "-" << std::endl;
            stream << std::setfill(' ');

            for(size_t sIndex = m.signalIndex; sIndex < m.signalIndex + m.signalCount; ++sIndex)
            {
                const CanSignal& s = m.signalArray[sIndex];

                stream << "   " << std::setw(32) << s.name;
                stream << " | " << std::setw(8)  << std::hex << s.bitMask;
                stream << " | " << std::setw(10) << std::dec << s.bitLength;
                stream << " | " << std::setw(12) << std::dec << s.bitPosition << std::endl;
            }

            stream << std::endl;
        }
    }
}