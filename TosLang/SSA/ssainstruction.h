#ifndef SSA_INSTRUCTION__TOSTITOS
#define SSA_INSTRUCTION__TOSTITOS

#include "ssavalue.h"

#include <cassert>
#include <vector>

namespace TosLang
{
    namespace BackEnd
    {
        template <class InstT>
        class BasicBlock;

        /*
        * TODO
        */
        class SSAInstruction
        {
        public:
            /*
            * TODO
            */
            enum class Operation
            {
                PHI,
                BR,
                CALL,
                RET,
                MOV,
                ADD,
                SUB,
                GT,
                LT,
                AND,
                OR,
                XOR,
                MUL,
                DIV,
                LSHIFT,
                RSHIFT,
                MOD,
                NOT,
                NEG,
                UNKNOWN,
            };

        public:
            SSAInstruction(Operation op, size_t valID) : mOp{ op }, mVal{ valID } { }
            virtual ~SSAInstruction() = default;

        public:
            friend std::ostream& operator<<(std::ostream& stream, const SSAInstruction& op);
            friend bool operator==(const SSAInstruction& lhsInst, const SSAInstruction& rhsInst);
            
        public:
            /*
            * TODO
            */
            Operation GetOperation() const { return mOp; }

            /*
            * TODO
            */
            BasicBlock<SSAInstruction>* GetBlock() const { return mBlock; }

            /*
            * TODO
            */
            void AddOperand(SSAValue val) { mOperands.push_back(val); }

            /*
            * TODO
            */
            const std::vector<SSAValue>& GetOperands() const { return mOperands; }

            /*
            * TODO
            */
            SSAValue GetReturnValue() const { return mVal; }

            /*
            * TODO
            */
            const std::vector<SSAInstruction*>& GetUsers() const { return mUsers; }

            /*
            * TODO
            */
            void AddUser(SSAInstruction* user) { mUsers.push_back(user); }

        private:
            Operation mOp;
            BasicBlock<SSAInstruction>* mBlock;     /*!< Block containing the instruction */
            std::vector<SSAValue> mOperands;        /*!< Operands of the instructions */
            std::vector<SSAInstruction*> mUsers;    /*!< Others instructions using the value produced by this instruction */
            SSAValue mVal;                          /*!< Value produced by the instruction */
        };
    
        std::ostream& operator<<(std::ostream& stream, const SSAInstruction& op);
        bool operator==(const SSAInstruction& lhsInst, const SSAInstruction& rhsInst);
    }
}

#endif //SSA_INSTRUCTION__TOSTITOS
