#include "registerallocator.h"

#include <map>

using namespace TosLang::Backend;

void RegisterAllocator::Allocate(std::vector<MachineEngine::ProcessorSpace::Instruction>& program) const
{
    std::map<unsigned, unsigned> virtRegCounts;

    for (auto& inst : program)
    {
        //virtRegCounts[]
    }
}
