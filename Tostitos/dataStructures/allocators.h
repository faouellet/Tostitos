#ifndef ALLOCATORS_H__TOSTITOS
#define ALLOCATORS_H__TOSTITOS

#include <limits>
#include <memory>
#include <new>
#include "dataStructures.h"
#include "../memory/userMemory.h"


// TODO if you are working on the file system (and have no intention of using the user memory)
// comment this file and the implementation file to be sure to not be limited by the available memory

void * operator new (std::size_t size)
{
    return UserMemory::Memory::GetInstance().requestMemory(size);
}

void * operator new[](std::size_t size)
{
    return UserMemory::Memory::GetInstance().requestMemory(size);
}

void operator delete (void * ptr)
{
    UserMemory::Memory::GetInstance().invalidateMemory(reinterpret_cast<unsigned char*>(ptr));
}

void operator delete[](void * ptr)
{
    UserMemory::Memory::GetInstance().invalidateMemory(reinterpret_cast<unsigned char*>(ptr));
}



#endif
