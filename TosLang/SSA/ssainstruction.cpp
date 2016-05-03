#include "ssainstruction.h"

#include <string>
#include <type_traits>

using namespace TosLang::BackEnd;

std::string OperationToStr(const SSAInstruction::Operation& operation)
{
    switch (operation)
    {
    case SSAInstruction::Operation::PHI: return "PHI";
    case SSAInstruction::Operation::BR: return "BR";
    case SSAInstruction::Operation::MOV: return "MOV";
    case SSAInstruction::Operation::CALL: return "CALL";
    case SSAInstruction::Operation::RET: return "RET";
    case SSAInstruction::Operation::ADD: return "ADD";
    case SSAInstruction::Operation::SUB: return "SUB";
    case SSAInstruction::Operation::GT: return "GT";
    case SSAInstruction::Operation::LT: return "LT";
    case SSAInstruction::Operation::AND: return "AND";
    case SSAInstruction::Operation::OR: return "OR";
    case SSAInstruction::Operation::XOR: return "XOR";
    case SSAInstruction::Operation::MUL: return "MUL";
    case SSAInstruction::Operation::DIV: return "DIV";
    case SSAInstruction::Operation::LSHIFT: return "LSHIFT";
    case SSAInstruction::Operation::RSHIFT: return "RSHIFT";
    case SSAInstruction::Operation::MOD: return "MOD";
    case SSAInstruction::Operation::NOT: return "NOT";
    case SSAInstruction::Operation::NEG: return "NEG";
    }

    return "UNKNOWN";
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const SSAInstruction& ssaInst)
{
    stream << OperationToStr(ssaInst.mOp) << " ";

    for (const auto& operand : ssaInst.mOperands)
        stream << operand << ", ";

    stream << ssaInst.mVal;

    return stream;
}

bool TosLang::BackEnd::operator==(const SSAInstruction& lhsInst, const SSAInstruction& rhsInst)
{
    return lhsInst.mBlock == rhsInst.mBlock
        && lhsInst.mOp == rhsInst.mOp
        && lhsInst.mVal == rhsInst.mVal
        && lhsInst.mOperands == rhsInst.mOperands
        && lhsInst.mUsers == rhsInst.mUsers;
}
