#ifndef CAN_DATABASE_H
#define CAN_DATABASE_H

// CAN Database ---------------------------------------------------------------------------------------------------------------
// Description: An interface for CAN bus communication. Received messages are parsed using a DBC file and stored in a
//   relational database for random access.

// To do:
// - Type safety is a big issue. Can possible implement runtime exceptions to prevent misuse.

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

        #define DEBUG_MODE

        // Constants ----------------------------------------------------------------------------------------------------------

        #define SOCKET_RX_TIMEOUT_MS 1000

        // Member Variables ---------------------------------------------------------------------------------------------------

        private:
        
        CanSocket txInterface;
        CanSocket rxInterface;

        pthread_t rxThread;

        CanMessage* messages;            // Message array
        CanSignal*  signals;             // Signal array. Parallel array to the database entry array.
        size_t      messageCount;        // Number of messages in the message array
        size_t      signalCount;         // Number of signals in the signal array

        bool rxThreadControl;            // Control / status of the RX thread. If set to false, the thread will terminate.
        bool rxThreadDebug;              // RX thread debug control. Only works if the DEBUG_MODE flag is set in compliation

        // Constructor / Destructor -------------------------------------------------------------------------------------------

        public:

        // Constructor
        // - Creates a CAN Database tied to the specified device and DBC file
        // - Creates a thread for scanning the CAN bus for incoming messages
        CanDatabase(std::string databaseFilePath, std::string canDeviceName);

        // Destructor
        // - Deallocates the database memory and terminates the RX thread
        ~CanDatabase();

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

        // Get CAN Messages
        // - Returns a reference to the database's CAN Messages
        // - Used for printing of CAN database messages / signals
        const std::vector<CanMessage>& getMessages() const;

        void printMessages(std::ostream& stream);

        // Scan RX
        // - Scan for messages using the provided database's rx socket
        // - Writes received data to the appropriate entry, ignores if unknown
        // - Will continue to scan until the RX control is set false  
        static void* scanRx(void* database_);

        // Print Messages
        static void printMessages(std::ostream& stream, CanMessage* messages, size_t messageCount);
    };
}

#endif // CAN_DATABASE_H