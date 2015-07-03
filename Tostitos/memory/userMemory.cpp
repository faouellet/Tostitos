#include "userMemory.h"

#include "../machine/machine.h"

using namespace UserMemory;

Memory::Memory()
{
    // right now, only the first page is used
    mCurrentPage = &MachineEngine::Machine::GetInstance().getMemory().RequestMemory(0);
}

unsigned char * Memory::requestMemory(size_t size)
{
    auto ptr = mCurrentPage->bytes + mOffset;
    size += INT_SIZE;

    *reinterpret_cast<unsigned int*>(ptr) = size;

    for (size_t i = 0; i < size; ++i)
    {
        mCurrentPage->validate[mOffset + i] = true;
    }

    mOffset += size;
    return ptr + INT_SIZE;
}

void Memory::invalidateMemory(unsigned char * ptr)
{
    auto startingPoint = ptr - INT_SIZE;
    size_t size = *startingPoint;

    auto index = startingPoint - mCurrentPage->bytes;

    for (size_t i = 0; i < size; ++i)
    {
        mCurrentPage->validate[index + i] = false;
    }
}