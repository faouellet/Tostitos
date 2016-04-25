#ifndef REGISTER_ALLOCATOR_H__TOSTITOS
#define REGISTER_ALLOCATOR_H__TOSTITOS

#include "../Common/cfgvisitor.h"

#include <vector>

namespace TosLang
{
    namespace Backend
    {
        /*
        * \class    RegisterAllocator
        * \brief    TODO
        */
        class RegisterAllocation
        {
		public:
            RegisterAllocation(unsigned nbPhysRegs, unsigned nbSpillRegs) : mNbPhysRegs{ nbPhysRegs }, mNbSpillRegs{ nbSpillRegs } { }

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
            //void Allocate(std::vector<MachineEngine::ProcessorSpace::Instruction>& program) const;
			
		private:
			unsigned mNbPhysRegs;
			unsigned mNbSpillRegs;
        };

        //using RegisterAllocator = Common::CFGVisitor<RegisterAllocation>;
    }
}

#endif // REGISTER_ALLOCATOR_H__TOSTITOS
