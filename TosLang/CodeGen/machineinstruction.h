#ifndef VIRTUAL_INSTRUCTION__TOSTITOS
#define VIRTUAL_INSTRUCTION__TOSTITOS

#include "MachineOperand.h"

#include <array>
#include <vector>

namespace TosLang
{
    namespace BackEnd
    {
        template <class InstT>
        class BasicBlock;

        /*
        * \class MachineInstruction
        * \brief Abstraction over a machine (Chip16) instruction.
        */
        class MachineInstruction
        {
        public:
            /*
            * \enum Opcode
            * \brief Possible opcodes for a VirtualInstruction
            */
            enum class Opcode
            {
                NO_OP,
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
            MachineInstruction() : mOpCode{ Opcode::UNKNOWN }, mNumOperands{ 0 }, mBlock{ nullptr }, mUsers{ } { }

            /*
            * \fn           VirtualInstruction
            * \brief        Ctor
            * \param opcode Opcode of the instruction
            * \param TODO
            */
            MachineInstruction(Opcode opcode, const BasicBlock<MachineInstruction>* block) : mOpCode{ opcode }, mNumOperands{ 0 }, mBlock{ block }, mUsers{ } { }

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
            MachineInstruction& AddImmOperand(unsigned op);

            /*
            * \fn       AddStackSlotOperand
            * \brief    Adds a stack slot number as an operand to the instruction
            * \param op Stack slot number
            */
            MachineInstruction& AddStackSlotOperand(unsigned op);

            /*
            * \fn       AddRegOperand
            * \brief    Adds a register as an operand to the instruction
            * \param op Register number
            */
            MachineInstruction& AddRegOperand(unsigned op);

            /*
            * \fn       AddTargetOperand
            * \brief    Adds a basic block as a target to a jump instruction
            * \param bb Target basic block
            */
            MachineInstruction& AddTargetOperand(BasicBlock<MachineOperand>* bb);

            /*
            * \fn               AddTargetOperand
            * \brief            Adds a function as a target to a call instruction
            * \param funcName   Name of the function to called
            */
            MachineInstruction& AddTargetOperand(const std::string& funcName);

        public:
            friend std::ostream& operator<<(std::ostream& stream, const MachineInstruction& inst);

        private:
            Opcode mOpCode;                             /*!< Instruction opcode */
            std::array<MachineOperand, 3> mOperands;    /*!< Instruction operands (can have up to 3) */
            unsigned short mNumOperands;                /*!< Number of operands the instruction currently has */
            const BasicBlock<MachineInstruction>* mBlock;   /*!< Block containing the instruction */
            std::vector<const MachineInstruction*> mUsers; /*!< TODO */
        };     
    }
}

#endif // VIRTUAL_INSTRUCTION__TOSTITOS
