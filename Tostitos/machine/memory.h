#ifndef MEMORY_H__TOSTITOS
#define MEMORY_H__TOSTITOS

#include "../common/typeDescriptions.h"
#include "../dataStructures/dataStructures.h"

#include <vector>

namespace MachineEngine
{
    namespace MemorySpace
    {
        class Memory
        {
        public:
            static const int NB_PAGES = 32;
        private:

            // the ram -- it must be a static array as this will not use the new operator
            DataStructure::MemoryPage mBytes[NB_PAGES];

        public:
            Memory()
            { 
            }

            DataStructure::MemoryPage & RequestMemory(int page)
            {
                return mBytes[page];
            }

            void FreeMemory(int page)
            {
                // nothing to do...or maybe it can be usefull to trace...
            }
        };
    };
};

#endif