#ifndef MACHINE_H__TOSTITOS
#define MACHINE_H__TOSTITOS

#include "cpu.h"
#include "hardDrive.h"
#include "memory.h"


#include <memory>
#include <functional>

namespace MachineEngine
{
    class Machine
    {
    public:
        static Machine & GetInstance()
        {
            static Machine instance;
            return instance;
        }

        MemorySpace::Memory & getMemory()
        {
            static MemorySpace::Memory memory;
            return memory;
        }

        ProcessorSpace::CPU & getCpu()
        {
            static ProcessorSpace::CPU cpu;
            return cpu;
        }

        DiskSpace::Disk & getHardDrive()
        {
            static DiskSpace::Disk disk;
            return disk;
        }
    };
}

#endif
