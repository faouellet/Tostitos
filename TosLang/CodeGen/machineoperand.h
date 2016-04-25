#ifndef VIRTUAL_OPERAND__TOSTITOS
#define VIRTUAL_OPERAND__TOSTITOS

#include <array>

namespace TosLang
{
    namespace BackEnd
    {
        /*
        * \class MachineOperand
        * \brief Abstraction over a Chip16 instruction operand.
        */
        class MachineOperand
        {
        public:
            enum class OperandKind
            {
                UNKNOWN,    // I am error
                FUNCTION,   // Function to be called
                IMMEDIATE,  // Immediate value folded into the operand
                STACK_SLOT, // Value is on the stack
                REGISTER,   // Value is inside a register
            };

        public:
            MachineOperand();
            MachineOperand(const unsigned op, const OperandKind kind);

        public:
            friend std::ostream& operator<<(std::ostream& stream, const MachineOperand& op);
            
        private:
            union
            {
                unsigned imm;
                unsigned stackslot;
                unsigned reg;
            };
            OperandKind mKind;
        };
    }
}

#endif // VIRTUAL_OPERAND__TOSTITOS
