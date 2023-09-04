// Header
#include "can_database.h"

// Includes
#include "log.h"

// C Standard Libraries
#include <string.h>
#include <errno.h>

namespace Network
{
    CanDatabase::CanDatabase(const char* databaseFilePath, const char* canDeviceName) : txInterface(canDeviceName), rxInterface(canDeviceName)
    {
        LOG_INFO("Creating CAN database bound to device '%s' using DBC file '%s'...\n", canDeviceName, databaseFilePath);
        
        CanDbc::parseFile(databaseFilePath, &messages, &signals, &messageCount, &signalCount);

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
                // Default to int and log error.
                LOG_ERROR("Failed to create CAN database: Signal '%s' has unknown datatype.\n", signal.name);
                int data = 0;
                this->insert(data, signal.name);
            }
        }

        rxInterface.setTimeoutMs(SOCKET_RX_TIMEOUT_MS);

        rxThreadControl = false;
        rxThreadDebug = false;

        this->startRxThread();
    }

    CanDatabase::~CanDatabase()
    {
        LOG_INFO("Terminating RX thread...\n");

        this->endRxThread();

        LOG_INFO("RX thread ended.\n");

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

        LOG_INFO("CAN database destroyed.\n");
    }

    template<typename T>
    void CanDatabase::send(const char* key, const T& value)
    {
        // Identify the parent message
        size_t targetIndex = find(key);
        CanMessage& message = messages[signals[targetIndex].messageIndex];

        // Update the database
        set(targetIndex, value);

        // Encode the message
        uint64_t messageData = 0;
        for(size_t index = message.signalIndex; index < message.signalIndex + message.signalCount; ++index)
        {
            CanSignal& signal = signals[index];

            switch(signal.datatypeId)
            {
                case ID_DATATYPE_UINT:
                    messageData |= CanSocket::encodeUnsignedInt(get<unsigned int>(index), signal);
                    break;
                case ID_DATATYPE_INT:
                    messageData |= CanSocket::encodeSignedInt(get<int>(index), signal);
                    break;
                case ID_DATATYPE_DOUBLE:
                    messageData |= CanSocket::encodeDouble(get<double>(index), signal);
                    break;
                case ID_DATATYPE_BOOL:
                    messageData |= CanSocket::encodeBool(get<bool>(index), signal);
                    break;
                default:
                    LOG_ERROR("Failed to send database message: The signal '%s' has an unknown datatype %i\n", key, signal.datatypeId);
                    throw std::runtime_error(std::string("Failed to send database message: The signal '") + signal.name + "' has an unknown datatype " + std::to_string(signal.datatypeId) + "'");
                    break;
            }
        }

        // Send the message
        uint8_t dataLength = 8; // TODO: Hard coded DLC of 8B
        txInterface.sendMessage(&messageData, &dataLength, &message.id);
    }

    void* CanDatabase::scanRx(void* database_)
    {
        LOG_INFO("Beginning RX thread...\n");
        
        CanDatabase* database = reinterpret_cast<CanDatabase*>(database_);

        uint16_t id;
        uint64_t data;
        uint8_t  dataLength;

        while(database->rxThreadControl)
        {
            if(database->rxThreadDebug)
            {
                LOG_INFO("RX thread scanning...\n");
            }
            
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
                            unsigned int signalData = static_cast<unsigned int>(CanSocket::decodeUnsignedInt(data, signal));
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_INT)
                        {
                            int signalData = static_cast<int>(CanSocket::decodeSignedInt(data, signal));
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_BOOL)
                        {
                            bool signalData = CanSocket::decodeBool(data, signal);
                            database->set(signalIndex, signalData);
                        }
                        else if(signal.datatypeId == ID_DATATYPE_DOUBLE)
                        {
                            double signalData = CanSocket::decodeDouble(data, signal);
                            database->set(signalIndex, signalData);
                        }
                    }

                    messageFound = true;
                }

                if(!messageFound && database->rxThreadDebug)
                {
                    LOG_WARN("Received unknown message, ID: %X. Ignoring...\n", id);
                }
            }
            catch(const PosixSocket::Timeout& exception) {}
            catch(const std::exception& exception)
            {
                LOG_WARN("RX thread error: %s\n", exception.what());
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

    void CanDatabase::print(char* buffer, size_t bufferSize)
    {
        int index = 0;

        index = snprintf(buffer, bufferSize, "%32s | %10s | %8s | %10s | %12s | %4s | %12s | %8s | %9s\n\n", "Signal Name", "Value", "Bit Mask", "Bit Length", "Bit Position", "Type", "Scale Factor", "Offset", "Is Signed");
        
        if(index <= 0)
        {
            LOG_ERROR("Failed to print CAN database: %s\n", strerror(errno));
            return;
        }
        if(static_cast<int>(bufferSize) - index <= 0)
        {
            LOG_ERROR("Failed to print CAN database: Buffer is too small.\n");
            return;
        }

        buffer += index;
        bufferSize -= index;

        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            const CanMessage& m = messages[mIndex];

            index = snprintf(buffer, bufferSize, "%s - ID: %3X\n", m.name, m.id);
            
            if(index <= 0)
            {
                LOG_ERROR("Failed to print CAN database: %s\n", strerror(errno));
                return;
            }
            if(static_cast<int>(bufferSize) - index <= 0)
            {
                LOG_ERROR("Failed to print CAN database: Buffer is too small.\n");
                return;
            }

            buffer += index;
            bufferSize -= index;

            for(size_t sIndex = m.signalIndex; sIndex < m.signalIndex + m.signalCount; ++sIndex)
            {
                const CanSignal& s = m.signalArray[sIndex];

                if(s.datatypeId == ID_DATATYPE_UINT)
                {
                    unsigned int data = get<unsigned int>(sIndex);
                    index = snprintf(buffer, bufferSize, "%32s | %10u | %8lX | %10i | %12i | %4i | %12f | %8f | %9u\n", s.name, data, s.bitMask, s.bitLength, s.bitPosition, s.datatypeId, s.scaleFactor, s.offset, s.signedness);
                }
                else if(s.datatypeId == ID_DATATYPE_INT)
                {
                    int data = get<int>(sIndex);
                    index = snprintf(buffer, bufferSize, "%32s | %10i | %8lX | %10i | %12i | %4i | %12f | %8f | %9u\n", s.name, data, s.bitMask, s.bitLength, s.bitPosition, s.datatypeId, s.scaleFactor, s.offset, s.signedness);
                }
                else if(s.datatypeId == ID_DATATYPE_BOOL)
                {
                    unsigned int data = static_cast<unsigned int>(get<bool>(sIndex));
                    index = snprintf(buffer, bufferSize, "%32s | %10u | %8lX | %10i | %12i | %4i | %12f | %8f | %9u\n", s.name, data, s.bitMask, s.bitLength, s.bitPosition, s.datatypeId, s.scaleFactor, s.offset, s.signedness);
                }
                else if(s.datatypeId == ID_DATATYPE_DOUBLE)
                {
                    double data = get<double>(sIndex);
                    index = snprintf(buffer, bufferSize, "%32s | %10f | %8lX | %10i | %12i | %4i | %12f | %8f | %9u\n", s.name, data, s.bitMask, s.bitLength, s.bitPosition, s.datatypeId, s.scaleFactor, s.offset, s.signedness);
                }
                else
                {
                    index = snprintf(buffer, bufferSize, "%32s | %10s | %8lX | %10i | %12i | %4i | %12f | %8f | %9u\n", s.name, "--", s.bitMask, s.bitLength, s.bitPosition, s.datatypeId, s.scaleFactor, s.offset, s.signedness);
                }

                if(index <= 0)
                {
                    LOG_ERROR("Failed to print CAN database: %s\n", strerror(errno));
                    return;
                }
                if(static_cast<int>(bufferSize) - index <= 0)
                {
                    LOG_ERROR("Failed to print CAN database: Buffer is too small.\n");
                    return;
                }

                buffer += index;
                bufferSize -= index;
            }

            index = snprintf(buffer, bufferSize, "\n");

            if(index <= 0)
            {
                LOG_ERROR("Failed to print CAN database: %s\n", strerror(errno));
                return;
            }
            if(static_cast<int>(bufferSize) - index <= 0)
            {
                LOG_ERROR("Failed to print CAN database: Buffer is too small.\n");
                return;
            }

            buffer += index;
            bufferSize -= index;
        }
    }

    // Template Forward Declarations ------------------------------------------------------------------------------------------

    template void CanDatabase::send(const char* key, const int& value);
    template void CanDatabase::send(const char* key, const unsigned int& value);
    template void CanDatabase::send(const char* key, const bool& value);
    template void CanDatabase::send(const char* key, const double& value);
}