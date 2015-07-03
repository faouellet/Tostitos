#ifndef USER_MEMORY_H__TOSTITOS
#define USER_MEMORY_H__TOSTITOS

#include "../common/typeDescriptions.h"
#include "../dataStructures/dataStructures.h"


// this must handle the call to memory for the page request
// it must contain the ttl as this will associate and liberate free pages as well as selectionning the best memory to allocate

namespace UserMemory
{
    class Memory
    {
    private:
        DataStructure::MemoryPage * mCurrentPage;
        int mOffset;

        // this is a default value to be plateform agnostic
        const int INT_SIZE = 4;

    private:
        Memory();
        Memory(const Memory &) = delete;
        void operator=(const Memory &) = delete;

    public:
        static Memory & GetInstance()
        {
            static Memory instance;
            return instance;
        }

        unsigned char * requestMemory(size_t size);
        void invalidateMemory(unsigned char * ptr);
    };
}

#endif