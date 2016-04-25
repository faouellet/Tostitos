#include "registerallocator.h"

#include <algorithm>
#include <unordered_map>

using namespace TosLang::Backend;

using RegCount = std::unordered_map<unsigned, unsigned>::value_type;

/*void RegisterAllocation::Allocate(std::vector<MachineEngine::ProcessorSpace::Instruction>& program) const
{
    std::unordered_map<unsigned, unsigned> virtRegCounts;

    for (auto& inst : program)
    {
		virtRegCounts[inst.GetFirstOperand()]++;

		if (inst.UseSecondOperand())
			virtRegCounts[inst.GetSecondOperand()]++;

		if (inst.UseThirdOperand())
			virtRegCounts[inst.GetThirdOperand()]++;
    }

	// The program doesn't use more virtual registers than they are physical registers.
	// Nothing to be done
	if (virtRegCounts.size() <= mNbPhysRegs)
		return;

	// Compute priority for the virtual registers
	//std::vector<RegCount> RegCounts(virtRegCounts.begin(), virtRegCounts.end());
	//std::sort(RegCounts.begin(), RegCounts.end(), [](RegCount r1, RegCount r2) { return r1.second > r2.first; });
}*/
