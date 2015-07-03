#ifndef HARD_DRIVE_H__TOSTITOS
#define HARD_DRIVE_H__TOSTITOS

#include "../dataStructures/dataStructures.h"
#include "../common/typeDescriptions.h"

#include <memory>
#include <string>
#include <vector>

namespace MachineEngine
{
    namespace DiskSpace
    {
        using Data = DataStructure::FileData;

        class Disk
        {
        public:
            static const int SECTOR_SIZE = 64;
            static const int NUMBER_OF_SECTORS = 1024;
            static const int TOTAL_SIZE = NUMBER_OF_SECTORS * SECTOR_SIZE;
        
        private:
            int mCurrentSector;
            Common::ByteArray mData;

        public:
            Disk();
            Disk(Common::ByteArray data);
            ~Disk();

            // rotate disk to needed sector
            // returns the time needed to rotate
            int Rotate(int sectorNumber);
            // read the current sector
            Data Read();
            
            // write an single disk sector. These routines send a request to 
            // the disk and return immediately. Only one request allowed at a time!
            void Write(Data data);

            // get the current Sector
            int GetCurrentSector() const;

        private:
            int GetIndex(int sectorNumber) const;
        };
    }
}

#endif