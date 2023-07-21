#ifndef CAN_DATABASE_H
#define CAN_DATABASE_H

// Includes
#include "database.h"
#include "can_socket.h"
#include "can_dbc.h"

// C++ Standard Libraries
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

// POSIX Libraries
#include <pthread.h>
#include <string.h>

class CanDatabase : public Database
{
    // Constants --------------------------------------------------------------------------------------------------------------

    #define SOCKET_RX_TIMEOUT_MS 1000

    // Member Variables -------------------------------------------------------------------------------------------------------

    private:
    
    CanSocket txInterface;
    CanSocket rxInterface;

    pthread_t rxThread;

    std::vector<CanDbc::CanMessage> messages;

    size_t** signalIndices;              // Array of arrays of signal indices, the base array correlates to each message, while
                                         // the internal arrays correlate to each message's signals. Each index points to a 
                                         // database entry.
    size_t messageCount;                 // Number of messages in database. Indicates size of signalIndices base array.

    bool continueScan;

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    CanDatabase(std::string databaseFilePath, std::string canDeviceName) : txInterface(canDeviceName), rxInterface(canDeviceName)
    {
        size_t signalCount = CanDbc::parseFile(databaseFilePath, messages);

        this->allocate(signalCount);

        messageCount = messages.size();
        signalIndices = new size_t*[messageCount];
        
        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            size_t sCount = messages[mIndex].signals.size();
            signalIndices[mIndex] = new size_t[sCount];
            
            for(size_t sIndex = 0; sIndex < sCount; ++sIndex)
            {
                CanDbc::CanSignal& s = messages[mIndex].signals[sIndex];
                if(s.typeId == ID_TYPE_INT)
                {
                    int data = 0;
                    this->insert(data, s.name);
                    signalIndices[mIndex][sIndex] = this->getIndex(s.name);
                }
                else if(s.typeId == ID_TYPE_UINT)
                {
                    unsigned int data = 0;
                    this->insert(data, s.name);
                    signalIndices[mIndex][sIndex] = this->getIndex(s.name);
                }
                else if(s.typeId == ID_TYPE_BOOL)
                {
                    bool data = false;
                    this->insert(data, s.name);
                    signalIndices[mIndex][sIndex] = this->getIndex(s.name);
                }
            }
        }

        rxInterface.setTimeout(SOCKET_RX_TIMEOUT_MS);

        continueScan = true;
        pthread_create(&rxThread, NULL, &CanDatabase::scanRx, this);
    }

    ~CanDatabase()
    {
        std::cout << "Terminating RX thread..." << std::endl;

        // Terminate thread
        continueScan = false;
        pthread_join(rxThread, NULL);

        std::cout << "RX thread ended." << std::endl;

        // Delete signal indices
        for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
        {
            delete [] signalIndices[mIndex];
        }
        delete [] signalIndices;
    }

    // TODO: This function CANNOT be thread safe
    static void* scanRx(void* database_)
    {
        std::cout << "RXTHREAD: Begin." << std::endl;

        CanDatabase* database = reinterpret_cast<CanDatabase*>(database_);

        std::cout << "RXTHREAD: Message count = " << database->messageCount << std::endl;

        unsigned int id;
        uint64_t data;
        unsigned int dataCount;

        while(database->continueScan)
        {
            try
            {
                database->rxInterface.read_m(id, data, dataCount);

                bool messageFound = false;

                for(size_t mIndex = 0; mIndex < database->messageCount; ++mIndex)
                {
                    if(id != database->messages[mIndex].id) continue;
                    
                    CanDbc::CanMessage& message = database->messages[mIndex];

                    size_t signalCount = message.signals.size();
                    for(size_t sIndex = 0; sIndex < signalCount; ++sIndex)
                    {
                        CanDbc::CanSignal& signal = message.signals[sIndex];

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

                if(!messageFound) std::cout << "Read unknown message, ID: " << std::hex << id << ". Ignoring...";
            }
            catch(const CanSocket::Timeout& exception)
            {
                // std::cout << "Timeout." << std::endl;
            }
            catch(const std::exception& exception)
            {
                std::cout << "RXTHREAD: " << exception.what();
            }
        }

        std::cout << "RXTHREAD: End." << std::endl;

        return NULL;
    }

    const std::vector<CanDbc::CanMessage>& getMessages() const
    {
        return this->messages;
    }
};

#endif // CAN_DATABASE_H