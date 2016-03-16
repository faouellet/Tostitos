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
                IMMEDIATE,  // Immediate value folded into the operand
                MEM_SLOT,
                REGISTER,   // Value is inside a register
                TARGET,     // Basic block to which we will jump
                UNKNOWN     // I am error
            };

        public:
            VirtualOperand() : mKind{ OperandKind::UNKNOWN } { }

            VirtualOperand(unsigned op, OperandKind kind);
            explicit VirtualOperand(BasicBlock* bb);

        public:
            friend std::ostream& operator<<(std::ostream& stream, const VirtualOperand& op);

        private:
            union 
            {
                unsigned imm;
                unsigned memslot;
                unsigned reg;
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
            enum class Opcode
            {
                NO_OP,
                ALLOCA,
                JUMP,
                CALL,
                RET,
                LOAD_IMM,
                LOAD,
                MOV,
                STORE,
                PUSH,
                POP,
                ADD_IMM,
                ADD,
                SUB_IMM,
                SUB,
                GT,
                LT,
                AND_IMM,
                AND,
                OR_IMM,
                OR,
                XOR_IMM,
                XOR,
                MUL_IMM,
                MUL,
                DIV_IMM,
                DIV,
                LSHIFT,
                RSHIFT,
                MOD_IMM,
                MOD,
                NOT_IMM,
                NOT,
                NEG_IMM,
                NEG,
                UNKNOWN,
            };

        public:
            VirtualInstruction() : mOpCode{ Opcode::UNKNOWN }, mNumOperands{ 0 } { }
            explicit VirtualInstruction(Opcode opcode) : mOpCode{ opcode }, mNumOperands { 0 } { }

        public:
            VirtualInstruction& AddImmOperand(unsigned op);
            VirtualInstruction& AddMemSlotOperand(unsigned op);
            VirtualInstruction& AddRegOperand(unsigned op);
            VirtualInstruction& AddTargetOperand(BasicBlock* bb);

        public:
            friend std::ostream& operator<<(std::ostream& stream, const VirtualInstruction& inst);

        private:
            Opcode mOpCode;
            std::array<VirtualOperand, 3> mOperands;
            unsigned short mNumOperands;
        };     
    }
}

#endif // VIRTUAL_INSTRUCTION__TOSTITOS
