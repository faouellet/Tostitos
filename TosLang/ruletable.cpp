#include "ruletable.h"

RuleTable::RuleTable()
{
    mRules.emplace_back(RES_ASSIGN, OP_ASSIGN, std::vector<OperandType>{ REGISTER, REGISTER }, Instruction::MOV);
}
