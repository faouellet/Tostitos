#ifndef VIRTUAL_INSTRUCTION__TOSTITOS
#define VIRTUAL_INSTRUCTION__TOSTITOS

#include "virtualoperand.h"

#include <array>

namespace TosLang
{
    namespace BackEnd
    {
        /*
        * \class VirtualInstruction
        * \brief Abstraction over a Chip16 instruction. While its opcode is representative of what will be generated, 
        *        its operand are either refering to virtual registers (which are infinite in number) or a basic block.
        */
        class VirtualInstruction
        {
        public:
            /*
            * \enum Opcode
            * \brief Possible opcodes for a VirtualInstruction
            */
            enum class Opcode
            {
                NO_OP,
                PHI,
                JUMP,
                CALL,
                RET,
                LOAD_IMM,
                LOAD_SP,
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
            /*
            * \fn VirtualInstruction
            * \brief Default ctor
            */
            VirtualInstruction() : mOpCode{ Opcode::UNKNOWN }, mNumOperands{ 0 } { }

            /*
            * \fn           VirtualInstruction
            * \brief        Ctor
            * \param opcode Opcode of the instruction
            */
            explicit VirtualInstruction(Opcode opcode) : mOpCode{ opcode }, mNumOperands { 0 } { }

        public:
            /*
            * \fn               AddTargetOperand
            * \brief            Adds a function as a target to a call instruction
            * \param funcName   Name of the function to called
            */
            Opcode GetOpcode() const { return mOpCode; }


        public:
            /*
            * \fn       AddImmOperand
            * \brief    Adds an immediate operand (i.e. a literal value) to the instruction
            * \param op Immediate operand
            */
            VirtualInstruction& AddImmOperand(unsigned op);

            /*
            * \fn       AddStackSlotOperand
            * \brief    Adds a stack slot number as an operand to the instruction
            * \param op Stack slot number
            */
            VirtualInstruction& AddStackSlotOperand(unsigned op);

            /*
            * \fn       AddRegOperand
            * \brief    Adds a register as an operand to the instruction
            * \param op Register number
            */
            VirtualInstruction& AddRegOperand(unsigned op);

            /*
            * \fn       AddTargetOperand
            * \brief    Adds a basic block as a target to a jump instruction
            * \param bb Target basic block
            */
            VirtualInstruction& AddTargetOperand(BasicBlock* bb);

            /*
            * \fn               AddTargetOperand
            * \brief            Adds a function as a target to a call instruction
            * \param funcName   Name of the function to called
            */
            VirtualInstruction& AddTargetOperand(const std::string& funcName);

        public:
            friend std::ostream& operator<<(std::ostream& stream, const VirtualInstruction& inst);

        private:
            Opcode mOpCode;                             /*!< Instruction opcode */
            std::array<VirtualOperand, 3> mOperands;    /*!< Instruction operands (can have up to 3) */
            unsigned short mNumOperands;                /*!< Number of operands the instruction currently has */
        };     
    }
}

#endif // VIRTUAL_INSTRUCTION__TOSTITOS
