#include "ruletable.h"

#include <algorithm>
#include <iterator>

using namespace TosLang::BackEnd;

RuleTable::RuleTable()
{
    // ( Assign ( = ( REG IMM ) ) LDI )
    mRules.emplace_back(RES_ASSIGN, OP_ASSIGN, std::vector<OperandType>{ REGISTER, IMMEDIATE }, Instruction::LOAD_IMM);
    // ( Assign ( = ( REG REG ) ) MOV )
    mRules.emplace_back(RES_ASSIGN, OP_ASSIGN, std::vector<OperandType>{ REGISTER, REGISTER }, Instruction::MOV);
    
    // ( Reg ( no-op ( IMMEDIATE ) ) LDI )
    mRules.emplace_back(RES_REGISTER, NO_OP, std::vector<OperandType>{ IMMEDIATE }, Instruction::LOAD_IMM);
}

// TODO: That seems a bit too costly in the long run
std::vector<Rule> RuleTable::MatchPattern(const std::pair<OperatorType, std::vector<OperandType>>& pattern) const
{
    std::vector<Rule> matchedRules;

    std::copy_if(mRules.begin(), mRules.end(), std::back_inserter(matchedRules), 
        [&pattern](const Rule& rule)
        {
            return rule.mOperator == pattern.first && rule.mOperands == pattern.second;
        });

    return matchedRules;
}
