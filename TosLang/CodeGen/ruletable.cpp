#include "ruletable.h"

#include <algorithm>
#include <iterator>

using namespace TosLang::BackEnd;

RuleTable::RuleTable()
{
    // ( Assign ( = ( REG IMM ) ) LDI )
    mRules.emplace_back(ResultType::RES_ASSIGN, 
                        OperatorType::OP_ASSIGN, std::vector<OperandType>{ OperandType::REGISTER, OperandType::IMMEDIATE }, 
                        Instruction::InstructionType::LOAD_IMM);
    // ( Assign ( = ( REG REG ) ) MOV )
    mRules.emplace_back(ResultType::RES_ASSIGN, 
                        OperatorType::OP_ASSIGN, std::vector<OperandType>{ OperandType::REGISTER, OperandType::REGISTER }, 
                        Instruction::InstructionType::MOV);
}

// TODO: That seems a bit too costly in the long run
std::vector<Rule> RuleTable::MatchPattern(std::pair<OperatorType, std::vector<OperandType>>&& pattern) const
{
    std::vector<Rule> matchedRules;

    std::copy_if(mRules.begin(), mRules.end(), std::back_inserter(matchedRules), 
        [&pattern](const Rule& rule)
        {
            return rule.mOperator == pattern.first && rule.mOperands == pattern.second;
        });

    return matchedRules;
}
