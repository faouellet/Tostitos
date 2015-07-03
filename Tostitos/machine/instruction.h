#ifndef INSTRUCTION_H__TOSTITOS
#define INSTRUCTION_H__TOSTITOS

#include <algorithm>
#include <string>
#include <vector>

namespace MachineEngine
{
    namespace ProcessorSpace
    {
        enum class OpType 
        {
            // Arithmetic instruction
            ADD, 
            DIV, 
            MUL, 
            SUB,

            // Memory instruction
            LOAD,
            STORE,

            // Stack instruction
            PUSH,
			POP,

            // Thread instruction
			FORK,
            BARRIER,
            SLEEP,

            // Test instruction
            EXP,
            PRINT
        };

        class Instruction
        {
			private:
				static const char * OP_CODES[13];

            private:
                OpType mOpType;
                unsigned int mValue;

            private:
                OpType StrToOT(const std::string & OpStr) 
                {
					return static_cast<OpType>(
						std::find_if(OP_CODES, OP_CODES + 13, [&OpStr](const char * OpCode){ return OpStr.compare(OpCode) == 0; }) -
						OP_CODES);
                }

            public:
                Instruction(const std::string & Op) : mOpType{StrToOT(Op)} {}
                Instruction(unsigned int Value) : mOpType{OpType::PUSH}, mValue{Value}
                {
                }
                ~Instruction() = default;

                OpType GetOptype() const { return mOpType; }
                unsigned int GetValue() const { return mValue; }
        };
    };
};

#endif
