#include "database.h"

// Standard Libraries
#include <exception>
#include <stdexcept>

Database::Database()
{
    this->dataCount = 0;
    this->insertionIndex = -1;
}

Database::Database(size_t dataCount_)
{
    this->dataCount = 0;
    this->insertionIndex = -1;

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
    this->dataArray = new void*[dataCount_];
    this->keyArray = new std::string[dataCount_];

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
        // Note that the free function does not destroy objects, only frees memory. If a class is stored at this location its
        // destructor will not be called. Here free is used as this memory was created using malloc, delete cannot be used on
        // a void pointer.
        free(dataArray[index]);
    }
    
    // Delete the database arrays
    delete [] dataArray;
    delete [] keyArray;
}

template<typename T>
T* Database::reference(const std::string& key) const
{
    size_t index = getIndex(key);

    return reinterpret_cast<T*>(dataArray[index]);
}

template<typename T>
void Database::insert(const T& data, const std::string& key)
{
    // Check data bounds
    if(insertionIndex >= dataCount) throw std::runtime_error("Database insertion error: Maximum entries reached.");

    // Check for duplicates
    for(int index = 0; index < insertionIndex; ++index)
    {
        if(keyArray[index] == key) throw std::runtime_error("Database insertion error: Key \"" + key + "\" already exists.");
    }

    // Allocate memory for the data
    // Note that malloc only reserves space in memory, if T is an object it will not be constructed. Here malloc is used as the
    // pointer is a void*. If the new operator was used the created memory could not be deleted with the delete operator, hence
    // malloc must be used.
    dataArray[insertionIndex] = malloc(sizeof(T));
    if(dataArray[insertionIndex] == NULL) throw std::runtime_error("Database insertion error: Could not allocate memory.");

    // Copy the value of the data (only works for primatives)
    *reinterpret_cast<T*>(dataArray[insertionIndex]) = data;

    // Copy the key value
    keyArray[insertionIndex] = key;

    // Update the insertion index
    ++insertionIndex;
}

size_t Database::getIndex(const std::string& key) const
{
    if(dataCount == 0 || insertionIndex == -1) throw std::runtime_error("Database lookup error: The database is empty.");

    for(size_t index = 0; index < insertionIndex; ++index)
    {
        if(keyArray[index] == key) return index;
    }

    throw std::runtime_error("Database lookup error: Could not find key '" + key + "'.");
}

template<typename T>
void Database::set(size_t index, const T& data)
{
    if(index >= insertionIndex) throw std::runtime_error("Database assignment error: Index " + std::to_string(index) + " is out of range");

    *reinterpret_cast<T*>(dataArray[index]) = data;
}

// Template Forward Declarations ----------------------------------------------------------------------------------------------
// - This is done so that the database can only be used for known datatypes

// Integer (Size >= 2 Bytes)
template int* Database::reference<int>(const std::string&) const;
template void Database::insert(const int&, const std::string&);
template void Database::set(size_t, const int&);

// Unsigned Integer (Size >= 2 Bytes)
template unsigned int* Database::reference<unsigned int>(const std::string&) const;
template void Database::insert(const unsigned int&, const std::string&);
template void Database::set(size_t, const unsigned int&);

// Long Integer (Size >= 4 Bytes)
template long int* Database::reference<long int>(const std::string&) const;
template void Database::insert(const long int&, const std::string&);
template void Database::set(size_t, const long int&);

// Unsigned Long Integer (Size >= 4 Bytes)
template unsigned long int* Database::reference<unsigned long int>(const std::string&) const;
template void Database::insert(const unsigned long int&, const std::string&);
template void Database::set(size_t, const unsigned long int&);

// Long Long Integer (Size >= 8 Bytes)
template long long int* Database::reference<long long int>(const std::string&) const;
template void Database::insert(const long long int&, const std::string&);
template void Database::set(size_t, const long long int&);

// Unsigned Long Long Integer (Size >= 8 Bytes)
template unsigned long long int* Database::reference<unsigned long long int>(const std::string&) const;
template void Database::insert(const unsigned long long int&, const std::string&);
template void Database::set(size_t, const unsigned long long int&);

// Boolean (Size >= 1 Bit)
template bool* Database::reference<bool>(const std::string&) const;
template void Database::insert(const bool&, const std::string&);
template void Database::set(size_t, const bool&);

// Char (Size = 1 Byte)
template char* Database::reference<char>(const std::string&) const;
template void Database::insert(const char&, const std::string&);
template void Database::set(size_t, const char&);

// Unsigned Char (Size = 1 Byte)
template unsigned char* Database::reference<unsigned char>(const std::string&) const;
template void Database::insert(const unsigned char&, const std::string&);
template void Database::set(size_t, const unsigned char&);

// Single-Precision Floating Point Number (Size = 4 Bytes)
template float* Database::reference<float>(const std::string&) const;
template void Database::insert(const float&, const std::string&);
template void Database::set(size_t, const float&);

// Double-Precision Floating Point Number (Size = 8 Bytes)
template double* Database::reference<double>(const std::string&) const;
template void Database::insert(const double&, const std::string&);
template void Database::set(size_t, const double&);