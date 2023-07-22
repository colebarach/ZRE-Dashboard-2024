// Header
#include "can_database.h"

// C++ Standard Libraries
#include <iostream>
#include <iomanip>

// POSIX Libraries
#include <string.h>

CanDatabase::CanDatabase(std::string databaseFilePath, std::string canDeviceName) : txInterface(canDeviceName), rxInterface(canDeviceName)
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

    this->startRxThread();
}

CanDatabase::~CanDatabase()
{
    std::cout << "Terminating RX thread..." << std::endl;

    this->endRxThread();

    std::cout << "RX thread ended." << std::endl;

    // Delete signal indices
    for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
    {
        delete [] signalIndices[mIndex];
    }
    delete [] signalIndices;
}

template<typename T>
void CanDatabase::assign(T& data, std::string key)
{
    // TODO: Kinda a weird problem, how do you do this without needing to search and find?
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

const std::vector<CanDbc::CanMessage>& CanDatabase::getMessages() const
{
    return this->messages;
}