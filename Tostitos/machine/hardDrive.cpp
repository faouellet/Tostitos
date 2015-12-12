#include "hardDrive.h"
#include "internalHelperFunctions.h"

#include <algorithm>
#include <assert.h>

using namespace MachineEngine::DiskSpace;

using std::string;
using std::unique_ptr;
using std::vector;

const int ROTATE_TIME_PER_SECTOR = 10; // in ms

Disk::Disk()
    : mCurrentSector(0)

{
    mData.reserve(TOTAL_SIZE);
    mData.assign(TOTAL_SIZE, static_cast<char>(0));
}

Disk::Disk(Common::ByteArray data)
    : mCurrentSector(0)
    , mData(data)
{
}

Disk::~Disk()
{

}

int Disk::Rotate(int sectorNumber)
{
    assert(sectorNumber >= 0 && sectorNumber < NUMBER_OF_SECTORS);

    int nbSectorToTravel = 0;

    if (sectorNumber > mCurrentSector)
    {
        nbSectorToTravel = sectorNumber - mCurrentSector;
    }
    else
    {
        // go around...then forward
        nbSectorToTravel = (NUMBER_OF_SECTORS - mCurrentSector) + sectorNumber;
    }

    unsigned int requiredTime = nbSectorToTravel * ROTATE_TIME_PER_SECTOR;

    // auto & currentSector = mCurrentSector;

    // TODO does not work
    MachineEngine::InternalFunctions::internalTimer(std::chrono::milliseconds(requiredTime),
                                                    [this, sectorNumber]
                                                        {
                                                            mCurrentSector = sectorNumber;
                                                        });
    return requiredTime;
}

Data Disk::Read()
{
    auto currentSector = mCurrentSector;
    auto index = GetIndex(currentSector);

    Data readData;
    readData.data.assign(&mData[index], &mData[index] + SECTOR_SIZE);
    readData.sectorNumber = currentSector;

    return readData;
}

void Disk::Write(Data data)
{
    auto currentSector = mCurrentSector;
    auto index = GetIndex(currentSector);

    std::copy_n(data.data.begin(), data.data.size(), &mData[index]);
}

int Disk::GetCurrentSector() const
{
    return mCurrentSector;
}

int Disk::GetIndex(int sectorNumber) const
{
    return sectorNumber * SECTOR_SIZE;
}



