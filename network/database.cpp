#include "database.h"

// C++ Standard Libraries
#include <exception>
#include <stdexcept>

// C Standard Libraries
#include "string.h"

namespace Network
{
    Database::Database()
    {
        this->dataCount = 0;
        this->insertionIndex = 0;
    }

    Database::Database(size_t dataCount_)
    {
        this->dataCount = 0;
        this->insertionIndex = 0;

        allocate(dataCount_);
    }

    Database::~Database()
    {
        // Deallocate memory if required
        if(dataCount != 0) deallocate();
    }

    void Database::allocate(size_t dataCount_)
    {
        // Check database is empty
        if(this->dataCount != 0) throw std::runtime_error("Database allocation failure: The database is not empty.");

        // Allocate memory
        this->entries = new DatabaseEntry[dataCount_];

        this->dataCount = dataCount_;
        this->insertionIndex = 0;
    }

    void Database::deallocate()
    {
        // Check that database does have entries
        if(this->dataCount == 0) throw std::runtime_error("Database deallocation failure: The database is already empty.");

        // Iterate the database entries and free the allocated memory
        for(size_t index = 0; index < insertionIndex; ++index)
        {
            // Note that the free function does not destroy objects, only frees memory. If a class is stored at this location
            // its destructor will not be called. Here free is used as this memory was created using malloc, delete cannot be
            // used on a void pointer.
            free(entries[index].dataPtr);
        }
        
        // Delete the database entries
        delete [] entries;
    }

    template<typename T>
    T Database::get(const char* key) const
    {
        size_t index = this->find(key);

        return *reinterpret_cast<T*>(entries[index].dataPtr);
    }

    template<typename T>
    void Database::set(const char* key, const T& data)
    {
        size_t index = this->find(key);

        *reinterpret_cast<T*>(entries[index].dataPtr) = data;
    }

    template<typename T>
    T* Database::reference(const char* key) const
    {
        size_t index = this->find(key);

        return reinterpret_cast<T*>(entries[index].dataPtr);
    }

    template<typename T>
    void Database::insert(const T& data, const char* key)
    {
        // Check data bounds
        if(insertionIndex >= dataCount) throw std::runtime_error("Database insertion error: Maximum entries reached.");

        // Check for duplicates
        for(size_t index = 0; index < insertionIndex; ++index)
        {
            if(strcmp(key, entries[index].key) == 0) throw std::runtime_error(std::string("Database insertion error: Key \"") + key + "\" already exists.");
        }

        // Allocate memory for the data
        // Note that malloc only reserves space in memory, if T is an object it will not be constructed. Here malloc is used as
        // the pointer is a void*. If the new operator was used the created memory could not be deleted with the delete
        // operator, hence malloc must be used.
        entries[insertionIndex].dataPtr = malloc(sizeof(T));
        if(entries[insertionIndex].dataPtr == NULL) throw std::runtime_error("Database insertion error: Could not allocate memory.");

        // Copy the value of the data (only works for primatives)
        *reinterpret_cast<T*>(entries[insertionIndex].dataPtr) = data;

        // Copy the key value
        strcpy(entries[insertionIndex].key, key);

        // Update the insertion index
        ++insertionIndex;
    }

    size_t Database::find(const char* key) const
    {
        if(dataCount == 0 || insertionIndex == 0) throw std::runtime_error("Database lookup error: The database is empty.");

        for(size_t index = 0; index < insertionIndex; ++index)
        {
            if(strcmp(key, entries[index].key) == 0) return index;
        }

        throw std::runtime_error(std::string("Database lookup error: Could not find key '") + key + "'.");
    }

    template<typename T>
    T Database::get(size_t index) const
    {
        if(index >= insertionIndex) throw std::runtime_error(std::string("Database entry get error: Index ") + std::to_string(index) + " is out of range.");

        return *reinterpret_cast<T*>(entries[index].dataPtr);
    }

    template<typename T>
    void Database::set(size_t index, const T& data)
    {
        if(index >= insertionIndex) throw std::runtime_error(std::string("Database entry set error: Index ") + std::to_string(index) + " is out of range.");

        *reinterpret_cast<T*>(entries[index].dataPtr) = data;
    }

    void Database::print(char* buffer, size_t bufferSize)
    {
        
    }

    // Template Forward Declarations ------------------------------------------------------------------------------------------
    
    // This is done so that the database can only be used for known datatypes

    // Integer (Size >= 2 Bytes)
    template int* Database::reference(const char*) const;
    template void Database::insert(const int&, const char*);
    template void Database::set(size_t, const int&);
    template int  Database::get(size_t) const;
    template void Database::set(const char*, const int&);
    template int  Database::get(const char*) const;

    // Unsigned Integer (Size >= 2 Bytes)
    template unsigned int* Database::reference<unsigned int>(const char*) const;
    template void Database::insert(const unsigned int&, const char*);
    template void Database::set(size_t, const unsigned int&);
    template unsigned int Database::get(size_t) const;
    template void Database::set(const char*, const unsigned int&);
    template unsigned int Database::get<unsigned int>(const char*) const;

    // Long Integer (Size >= 4 Bytes)
    template long int* Database::reference<long int>(const char*) const;
    template void Database::insert(const long int&, const char*);
    template void Database::set(size_t, const long int&);
    template long int Database::get(size_t) const;
    template void Database::set(const char*, const long int&);
    template long int Database::get<long int>(const char*) const;

    // Unsigned Long Integer (Size >= 4 Bytes)
    template unsigned long int* Database::reference<unsigned long int>(const char*) const;
    template void Database::insert(const unsigned long int&, const char*);
    template void Database::set(size_t, const unsigned long int&);
    template unsigned long int Database::get(size_t) const;
    template void Database::set(const char*, const unsigned long int&);
    template unsigned long int Database::get<unsigned long int>(const char*) const;

    // Long Long Integer (Size >= 8 Bytes)
    template long long int* Database::reference<long long int>(const char*) const;
    template void Database::insert(const long long int&, const char*);
    template void Database::set(size_t, const long long int&);
    template long long int Database::get(size_t) const;

    // Unsigned Long Long Integer (Size >= 8 Bytes)
    template unsigned long long int* Database::reference<unsigned long long int>(const char*) const;
    template void Database::insert(const unsigned long long int&, const char*);
    template void Database::set(size_t, const unsigned long long int&);
    template unsigned long long int Database::get(size_t) const;

    // Boolean (Size >= 1 Bit)
    template bool* Database::reference<bool>(const char*) const;
    template void Database::insert(const bool&, const char*);
    template void Database::set(size_t, const bool&);
    template bool Database::get(size_t) const;
    template void Database::set(const char*, const bool&);
    template bool Database::get<bool>(const char*) const;

    // Char (Size = 1 Byte)
    template char* Database::reference<char>(const char*) const;
    template void Database::insert(const char&, const char*);
    template void Database::set(size_t, const char&);
    template char Database::get(size_t) const;
    template void Database::set(const char*, const char&);
    template char Database::get<char>(const char*) const;

    // Unsigned Char (Size = 1 Byte)
    template unsigned char* Database::reference<unsigned char>(const char*) const;
    template void Database::insert(const unsigned char&, const char*);
    template void Database::set(size_t, const unsigned char&);
    template unsigned char Database::get(size_t) const;
    template void Database::set(const char*, const unsigned char&);
    template unsigned char Database::get<unsigned char>(const char*) const;

    // Single-Precision Floating Point Number (Size = 4 Bytes)
    template float* Database::reference<float>(const char*) const;
    template void Database::insert(const float&, const char*);
    template void Database::set(size_t, const float&);
    template float Database::get(size_t) const;
    template void Database::set(const char*, const float&);
    template float Database::get<float>(const char*) const;

    // Double-Precision Floating Point Number (Size = 8 Bytes)
    template double* Database::reference<double>(const char*) const;
    template void Database::insert(const double&, const char*);
    template void Database::set(size_t, const double&);
    template double Database::get(size_t) const;
    template void Database::set(const char*, const double&);
    template double Database::get<double>(const char*) const;
}