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

// TODO:
// - Dynamic allocation for DB, used in inheritance.

// Standard Libraries
#include <string>

class Database
{
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
    // - This can only be called if the number of entries has not been specified
    void allocate(size_t dataCount_);

    // De-allocate
    // - Use this to de-allocate the entries of the database
    void deallocate();

    // Get Reference
    // - Call to get a pointer to an entry
    // - Use the type to identify the datatype
    // - Throws a standard exception if the entry does not exist
    template<typename T>
    T* reference(const std::string& key) const;

    // Insert Data
    // - Call to insert an entry into the database
    // - Inserts a copy of the data into the database
    // - Can only be used for primative objects
    // - Will throw a standard exception if insertion fails
    template<typename T>
    void insert(const T& data, const std::string& key);

    // Private Variables ------------------------------------------------------------------------------------------------------

    protected:

    void**       dataArray;              // Array of void pointers, each element points to an arbitrary variable
    std::string* keyArray;               // Array of keys, the index of each matches that of the data it is paired with

    size_t       dataCount;              // Data count, stores the max number of possible entries
    size_t       insertionIndex;         // Insertion index, used to indicate the next empty space in the database

    // Private Functions ------------------------------------------------------------------------------------------------------

    // Get Index
    // - Use to get the index of a specified key
    // - Returns the index if it exists
    // - Throws a standard exception if the key does not exist
    size_t getIndex(const std::string& key) const;

    // Set Value
    // - Use to set the value of an entry using its index
    template<typename T>
    void set(size_t index, const T& data);
};

#endif // DATABASE_H