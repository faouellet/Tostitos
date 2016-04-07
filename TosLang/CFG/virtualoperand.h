#ifndef VIRTUAL_OPERAND__TOSTITOS
#define VIRTUAL_OPERAND__TOSTITOS

#include <array>

namespace TosLang
{
    namespace BackEnd
    {
        class BasicBlock;

        /*
        * \class VirtualOperand
        * \brief Abstraction over a Chip16 instruction operand.
        */
        class VirtualOperand
        {
        public:
            enum class OperandKind
            {
                UNKNOWN,    // I am error
                FUNCTION,   // Function to be called
                IMMEDIATE,  // Immediate value folded into the operand
                STACK_SLOT, // Value is on the stack
                REGISTER,   // Value is inside a register
                TARGET,     // Basic block to which we will jump
            };

        public:
            VirtualOperand();
            VirtualOperand(unsigned op, OperandKind kind);
            explicit VirtualOperand(BasicBlock* bb);
            explicit VirtualOperand(const std::string& funcName);

            VirtualOperand(const VirtualOperand& op);

            ~VirtualOperand();

        public:
            VirtualOperand& operator=(const VirtualOperand& op);
            friend std::ostream& operator<<(std::ostream& stream, const VirtualOperand& op);
            
        private:
            void Swap(VirtualOperand& op);

        private:
            union
            {
                unsigned imm;
                unsigned memslot;
                unsigned reg;
                BasicBlock* target;
                std::string fName;
            };
            OperandKind mKind;
        };
    }
}

#endif // VIRTUAL_OPERAND__TOSTITOS
