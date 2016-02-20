#ifndef VIRTUAL_INSTRUCTION__TOSTITOS
#define VIRTUAL_INSTRUCTION__TOSTITOS

#include "../../Tostitos/machine/instruction.h"

#include <array>

using MachineEngine::ProcessorSpace::Instruction;

namespace TosLang
{
    namespace BackEnd
    {
        class BasicBlock;

        /*
        * \class VirtualOperand
        * \brief TODO
        */
        class VirtualOperand
        {
        public:
            enum class OperandKind
            {
                IMMEDIATE,
                MEM_SLOT,
                REGISTER,
                TARGET,
                UNKNOWN
            };

        public:
            VirtualOperand() : mKind{ OperandKind::UNKNOWN } { }

            VirtualOperand(unsigned op, OperandKind kind);
            explicit VirtualOperand(BasicBlock* bb);

        private:
            union 
            {
                unsigned reg;
                unsigned imm;
                BasicBlock* target; 
            } mOperand;

            OperandKind mKind;
        };

        /*
        * \class VirtualInstruction
        * \brief Abstraction over a Chip16 instruction. While its opcode is representative of what will be generated, 
        *        its operand are either refering to virtual registers (which are infinite in number) or a basic block.
        */
        class VirtualInstruction
        {
        public:
            explicit VirtualInstruction(Instruction::InstructionOpCode opcode) : mOpCode{ opcode }, mNumOperands { 0 } { }

        public:
            VirtualInstruction& AddImmOperand(unsigned op);
            VirtualInstruction& AddMemSlotOperand(unsigned op);
            VirtualInstruction& AddRegOperand(unsigned op);
            VirtualInstruction& AddTargetOperand(BasicBlock* bb);

        private:
            Instruction::InstructionOpCode mOpCode;
            std::array<VirtualOperand, 3> mOperands;
            unsigned short mNumOperands;
        };     
    }
}

#endif // VIRTUAL_INSTRUCTION__TOSTITOS
