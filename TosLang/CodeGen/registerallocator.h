#ifndef REGISTER_ALLOCATOR_H__TOSTITOS
#define REGISTER_ALLOCATOR_H__TOSTITOS

#include "../../Tostitos/machine/instruction.h"

#include <vector>

namespace TosLang
{
    namespace Backend
    {
        /*
        * \class    RegisterAllocator
        * \brief    Last step of the compilation process, this class encapsulate the logic to allocate 
        *           physical registers for every instruction in a program.
        */
        class RegisterAllocator
        {
        public:
            /*
            * \fn               Allocate
            * \brief            Rewrites a program so it make use of physical register instead of virtual ones
            * \param program    A list of instruction defining a program. The registers used by this 
            *                   program are all considered virtual and may be replaced by physical one.
            *                   This function may introduce new instruction in the program when it needs 
            *                   to spill variables.
            *                   The current version of this algorithm is a simple local top-down allocator.
            */
            void Allocate(std::vector<MachineEngine::ProcessorSpace::Instruction>& program) const;
        };
    }
}

#endif // REGISTER_ALLOCATOR_H__TOSTITOS
