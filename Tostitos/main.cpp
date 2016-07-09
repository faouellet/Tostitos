#include "kernel/kernel.h"
#include "machine/hardDrive.h"
#include "communicationWithHost/hardDriveHelper.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <assert.h>

#include "system.h"

using namespace MachineEngine::DiskSpace;
using namespace KernelSpace;
using namespace Threading;

void TestRotate()
{
    Disk disk;
    int toSector = 25;

    auto timeToWait = disk.Rotate(toSector);

    std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait) + std::chrono::seconds(1));

    assert(toSector == disk.GetCurrentSector());
}

void TestRead()
{
    Disk disk;

    auto data = disk.Read();

    for (auto & elem : data.data)
    {
        assert(elem == static_cast<char>(0));
    }

    assert(data.data.size() == Disk::SECTOR_SIZE);
    assert(data.sectorNumber == 0);
}

void TestWrite()
{
    Disk disk;

    auto timeToWait = disk.Rotate(45);

    std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait) + std::chrono::seconds(1));


    auto data = disk.Read();
    
    auto value = 0x0;
    for (auto & elem : data.data)
    {
        elem = value;
        ++value;
    }

    disk.Write(data);

    auto dataRead = disk.Read();

    for (int i = 0; i < Disk::SECTOR_SIZE; ++i)
    {
        assert(dataRead.data[i] == data.data[i]);
    }
}

void TestCommunicationReadWrite()
{
    const int TEST_VALUE = 25;
    std::string hardDrivePath = "hardDrive.txt";

    Common::ByteArray dataInto;

    for (unsigned char i = 0; i < TEST_VALUE; ++i)
    {
        dataInto.push_back(i);
    }

    CommunicationWithHost::WtiteToHost(hardDrivePath, dataInto);

    auto dataRead = CommunicationWithHost::ReadFromHost(hardDrivePath);

    assert(dataRead.size() == dataInto.size() && dataRead.size() == TEST_VALUE);

    for (int i = 0; i < TEST_VALUE; ++i)
    {
        assert(dataRead[i] == dataInto[i]);
    }
}

void TestKernel()
{
    //Kernel::GetInstance().RunProgram("scripts/memory.tos");
}

int main()
{
    std::cout << "Hello World" << std::endl;
    //TestRotate();
    //TestRead();
    //TestWrite();

    //TestCommunicationReadWrite();

    TestKernel();

    //std::cin.get();
}
