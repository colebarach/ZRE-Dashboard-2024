#ifndef DATABASE_H
#define DATABASE_H

// Relational Primative Database ----------------------------------------------------------------------------------------------
// Author: Cole Barach
// Date Created: 23.07.12
// Date Updated: 23.07.12
//   A relational database for primative datatypes. Entries are stored as a key/value pair, similar to the standard map. The
//   difference between this and the standard map is that entries can be any primate datatype. To access a value, the reference
//   function will return a pointer to the data which can be reused as needed. This pointer will be valid until the database
//   is destroyed. Note that as memory management is done manually, non-primative objects cannot be stored without causing
//   memory leak. An inserted object can only be freed, not destroyed.

// To do:
// - Test for logic errors and memory leaks. I think this is all good, but things were changed recently.

// C Standard Libraries
#include "stddef.h"

class Database
{
    // Constants --------------------------------------------------------------------------------------------------------------

    #define DATABASE_KEY_LENGTH 64       // Maximum length of a database key

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    // Constructor
    // - Use to create an empty database. The number of entries must be specified using the allocate function before inserting
    //   any data
    Database();

    // Constructor
    // - Use to create a database with space for the specified number of entries
    Database(size_t dataCount_);

    // Destructor
    // - Frees the allocated memory for both entries and the data arrays
    ~Database();

    // Public Functions -------------------------------------------------------------------------------------------------------

    // Allocate
    // - Use to allocate a number of entries in the database
    // - This can only be called if the number of entries has not been specified already
    void allocate(size_t dataCount_);

    // De-allocate
    // - Use this to de-allocate the entries of the database
    void deallocate();

    // Get Reference
    // - This function should be used for repeated access to a single entry. If a get call needs to happen more than once, a 
    //   reference should be used instead.
    // - Call to get a pointer to an entry
    // - Use the type to identify the datatype
    // - Throws a standard exception if the entry does not exist
    template<typename T>
    T* reference(const char* key) const;

    // Get Value
    template<typename T>
    T get(const char* key) const;

    // Set Value
    template<typename T>
    void set(const char* key, const T& data);
    
    // Insert Data
    // - Call to insert an entry into the database
    // - Inserts a copy of the data into the database
    // - Can only be used for primative objects
    // - Will throw a standard exception if insertion fails
    template<typename T>
    void insert(const T& data, const char* key);

    // Protected Objects ------------------------------------------------------------------------------------------------------
    
    protected:

    struct DatabaseEntry
    {
        char key[DATABASE_KEY_LENGTH];   // Database key, c-string of max size DATABASE_KEY_LENGTH - 1
        void* dataPtr;                   // Pointer to arbitrary data
        size_t dataLength;               // Byte size of the data pointed to
    };

    // Protected Variables ----------------------------------------------------------------------------------------------------

    DatabaseEntry* entries;              // Dynamically allocated array of database entries
    size_t         dataCount;            // Data count, stores the max number of possible entries
    size_t         insertionIndex;       // Insertion index, used to indicate the next empty space in the database

    // Protected Functions ----------------------------------------------------------------------------------------------------

    // Find Index
    // - Use to get the index of a specified key
    // - Returns the index if it exists
    // - Throws a standard exception if the key does not exist
    size_t find(const char* key) const;

    // Get Value
    // - Use to get the value of an entry using its index
    template<typename T>
    void get(size_t index, T& data) const;

    // Set Value
    // - Use to set the value of an entry using its index
    template<typename T>
    void set(size_t index, const T& data);
};

#endif // DATABASE_H