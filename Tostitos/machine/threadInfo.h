#ifndef THREAD_INFO_H__TOSTITOS
#define THREAD_INFO_H__TOSTITOS

#include "instruction.h"

#include <stack>
#include <vector>

namespace MachineEngine
{
    namespace ProcessorSpace
    {
        enum class ThreadAction
        {
            DONE,
            FORK,
            BARRIER,
            SLEEP,
            RUN,
            EXP
        };

        struct ThreadContext
        {
            unsigned int PC;
            std::stack<unsigned int> Stack;
            std::vector<MachineEngine::ProcessorSpace::Instruction> Program;
			std::vector<unsigned char> Memory;
		};
    }
}

#endif