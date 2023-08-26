#ifndef CAN_DATABASE_H
#define CAN_DATABASE_H

// CAN Database ---------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: An interface for CAN bus communication. Received messages are parsed using a DBC file and stored in a
// relational database for random access.
//
// Created: 23.07.21
// Updated: 23.08.22
//
// To do:
// - Type safety is a big issue. Can possible implement runtime exceptions to prevent misuse.
// - The printing should be updated to a C-Style approach.

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "database.h"
#include "can_socket.h"
#include "can_dbc.h"

// C++ Standard Libraries
#include <vector>
#include <string>
#include <iostream>

// POSIX Libraries
#include <pthread.h>

namespace Network
{
    class CanDatabase : public Database
    {
        // Compilation Flags --------------------------------------------------------------------------------------------------

        #define DEBUG_GENERAL
        #define DEBUG_TRAFFIC

        // Constants ----------------------------------------------------------------------------------------------------------

        #define SOCKET_RX_TIMEOUT_MS 1000

        #define WIDTH_SIGNAL_NAME 32

        // Member Variables ---------------------------------------------------------------------------------------------------

        private:
        
        CanSocket txInterface;           // Socket for transmitting to the CAN bus.
        CanSocket rxInterface;           // Socket for reading from the CAN bus.

        pthread_t rxThread;              // Thread for scanning the receiving socket.

        CanMessage* messages;            // Array of CAN messages
        CanSignal*  signals;             // Signal array. Parallel array to the database entry array.
        size_t      messageCount;        // Number of messages in the message array.
        size_t      signalCount;         // Number of signals in the signal array.

        bool rxThreadControl;            // Control / status of the RX thread. If set to false, the thread will terminate.
        bool rxThreadDebug;              // RX thread debug control. Only works if the DEBUG_MODE flag is set in compliation.

        // Constructor / Destructor -------------------------------------------------------------------------------------------

        public:

        // Constructor
        // - Creates a CAN Database tied to the specified device and DBC file
        // - Creates a thread for scanning the CAN bus for incoming messages
        CanDatabase(const char* databaseFilePath, const char* canDeviceName);

        // Destructor
        // - Deallocates the database memory and terminates the RX thread
        ~CanDatabase();

        // TX -----------------------------------------------------------------------------------------------------------------

        // Send Value
        // - Call to updated a value in the database and transmit its associated message
        template<typename T>
        void send(const char* key, const T& value);

        // RX Thread ----------------------------------------------------------------------------------------------------------

        // Start RX Thread
        // - Call to start the database's RX thread, if not already active
        // - Will create the thread and start scanning
        // - Does nothing if the thread is already scanning
        void startRxThread();

        // End RX Thread
        // - Call to terminate the database's RX thread, if currently running
        // - Will set the control flag and wait until termination
        // - Does nothing if the thread is not running
        void endRxThread();

        // Get RX Thread Status
        // - Returns the state of the RX thread
        // - Returns true if scanning, false otherwise
        bool getRxThreadStatus() const;

        // Set RX Thread Debug
        // - Enable / disable RX thread debugging
        // - See rxThreadDebug for more info
        void setRxThreadDebug(bool state);

        // Get RX Thread Debug
        // - Get the state of the RX debug flag
        // - See rxThreadDebug for more info
        bool getRxThreadDebug() const;

        // Scan RX
        // - Scan for messages using the provided database's rx socket
        // - Writes received data to the appropriate entry, ignores if unknown
        // - Will continue to scan until the RX control is set false  
        static void* scanRx(void* database_);

        // Accessors ----------------------------------------------------------------------------------------------------------

        // Get Messages
        // - Call to get a read-only reference to the message array
        const CanMessage* getMessages() const { return this->messages; }

        // Get Signals
        // - Call to get a read-only reference to the signal array
        const CanSignal* getSignals() const { return this->signals; }

        // Get Message Count
        // - Call to get the number of elements in the message array
        size_t getMessageCount() const { return this->messageCount; }

        // Get Signal Count
        // - Call to get the number of elements in the signal array
        size_t getSignalCount() const { return this->signalCount; }

        void print(char* buffer, size_t bufferSize) override;
    };
}

#endif // CAN_DATABASE_H