#ifndef DATA_STRUCTURE_FILE_H__TOSTITOS
#define DATA_STRUCTURE_FILE_H__TOSTITOS

#include "../common/typeDescriptions.h"

#include <memory>


namespace DataStructure
{
    struct FileData
    {
        Common::ByteArray data;
        int sectorNumber;
    };

    struct MemoryPage
    {
        // 1 MB per page
        static const int BYTES_PER_PAGE = 131072;

        unsigned char bytes[BYTES_PER_PAGE];
        bool validate[BYTES_PER_PAGE];

        int offset;

        MemoryPage()
            :offset(0)
        {
            for (int i = 0; i < BYTES_PER_PAGE; ++i)
            {
                validate[i] = false;
            }
        }
    };
}

#endif