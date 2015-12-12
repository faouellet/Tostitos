#ifndef RULE_TABLE_H__TOSTITOS
#define RULE_TABLE_H__TOSTITOS

#include "../../Tostitos/machine/instruction.h"

#include <vector>
#include <tuple>

using MachineEngine::ProcessorSpace::Instruction;

namespace TosLang
{
    namespace BackEnd
    {
        enum class OperandType : unsigned int
        {
            IMMEDIATE, LABEL, REGISTER, UNKNOWN
        };

        enum class OperatorType : unsigned int
        {
            OP_ASSIGN, /*ADD, DIV, MOD, MUL, SUB*/ NO_OP
        };

        enum class ResultType : unsigned int
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
        *           which is then represented as the following expression for later matching:
        *                                               ( + ( RX IMM ) )
        */
        struct Rule
        {
            /*
            * \fn       Rule
            * \brief    Constructor
            */
            Rule(ResultType result = ResultType::RES_UNKNOWN, OperatorType op = OperatorType::NO_OP,
                std::vector<OperandType> operands = std::vector<OperandType>{},
                Instruction::InstructionType instType = Instruction::UNKNOWN) :
                mResult{ result }, mOperator{ op }, mInstType{ instType }
            {
                mOperands = operands;   // Is not in the initialization list because VS2013 won't accept it
                static unsigned ID = 0;
                mID = ID++;
            }

            /* The 2 following data members represent the pattern against which will try to match a part of the AST.
               They're essentially a tree pattern linearized as an S-expression.  */

            OperatorType mOperator;                 /*!< Operator in the pattern */
            std::vector < OperandType > mOperands;  /*!< Operands used in th pattern */

            ResultType  mResult;                    /*!< Type of the result produced by the pattern */
            Instruction::InstructionType mInstType; /*!< Chip16 instruction to be produced for the given pattern */
            unsigned    mID;                        /*!< ID of the rule */
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
            std::vector<Rule> MatchPattern(std::pair<OperatorType, std::vector<OperandType>>&& pattern) const;

            const Rule& GetRule(const unsigned id) const { return mRules[id]; }

        private:
            std::vector<Rule> mRules;    /*!< Tree rewriting rules. */
        };
    }
}

#endif // RULE_TABLE_H__TOSTITOS