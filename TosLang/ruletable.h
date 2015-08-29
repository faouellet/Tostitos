#ifndef RULE_TABLE_H__TOSTITOS
#define RULE_TABLE_H__TOSTITOS

#include "../Tostitos/machine/instruction.h"

#include <vector>
#include <tuple>

using MachineEngine::ProcessorSpace::Instruction;

namespace TosLang
{
    namespace BackEnd
    {
        enum OperandType
        {
            IMMEDIATE, LABEL, REGISTER, UNKNOWN
        };

        enum OperatorType
        {
            OP_ASSIGN, /*ADD, DIV, MOD, MUL, SUB*/ NO_OP
        };

        enum ResultType
        {
            RES_ASSIGN, RES_REGISTER, RES_UNKNOWN
        };

        /*
        * \class    Rule
        * \brief    A rule is a tree pattern that when matched against an AST will produce machine instruction.
        *           The tree pattern is a representation of an instruction in the target machine ISA. For example,
        *           the following Chip16 instruction:
        *                                               ADDI RX, HHLL
        *           will produce the following tree pattern
        *                                               (RY) +
        *                                                  /   \
        *                                                RX    IMM
        */
        struct Rule
        {
            /*
            * \fn       Rule
            * \brief    Constructor
            */
            Rule(ResultType result = RES_UNKNOWN, OperatorType op = NO_OP,
                std::vector<OperandType> operands = std::vector<OperandType>{},
                Instruction::InstructionType inst = Instruction::UNKNOWN) :
                mResult{ result }, mOperator{ op }, mInst{ static_cast<UInt32>(inst) }
            {
                mOperands = operands;   // Is not in the initialization list because VS2013 won't accept it
            }

            /* The 3 following data members represent the pattern against which will try to match a part of the AST.
            They're essentially a tree pattern linearized as an S-expression.  */

            ResultType mResult;                     /*!< Type of the result produced by the pattern */
            OperatorType mOperator;                 /*!< Operator in the pattern */
            std::vector < OperandType > mOperands;  /*!< Operands used in th pattern */

            Instruction mInst;                      /*!< Chip16 instruction to be produced for the given pattern */
        };

        /*
        * \class    RuleTable
        * \brief    Repository of all the rules of the Chip16 architecture.
        */
        class RuleTable
        {
        public:
            RuleTable();
            ~RuleTable() = default;

        public:
            /*
            * \fn               MatchPattern
            * \param pattern    Tree pattern to match
            * \brief            Find all the rules that match against the specified pattern
            * \return           Rules that matched against the pattern
            */
            std::vector<Rule> MatchPattern(const std::pair<OperatorType, std::vector<OperandType>>& pattern) const;

        private:
            std::vector<Rule> mRules;    /*!< Tree rewriting rules. */
        };
    }
}

#endif // RULE_TABLE_H__TOSTITOS