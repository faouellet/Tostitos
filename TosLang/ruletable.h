#ifndef RULE_TABLE_H__TOSTITOS
#define RULE_TABLE_H__TOSTITOS

#include "../Tostitos/machine/instruction.h"

#include <tuple>
#include <vector>

using MachineEngine::ProcessorSpace::Instruction;

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

struct Rule
{
    Rule(ResultType result = RES_UNKNOWN, OperatorType op = NO_OP, std::vector<OperandType> operands = std::vector<OperandType>{},
        Instruction::InstructionType inst = Instruction::UNKNOWN) : Inst{ inst }
    {
        Pattern = std::make_tuple(result, op, operands);
        
    }

    Instruction Inst;
    std::tuple<ResultType, OperatorType, std::vector<OperandType>> Pattern;
};

class RuleTable
{
public:
    RuleTable();
    ~RuleTable() = default;

private:
    std::vector<Rule> mRules;
};

#endif // RULE_TABLE_H__TOSTITOS