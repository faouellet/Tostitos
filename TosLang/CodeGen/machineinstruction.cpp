#include "machineinstruction.h"

#include "basicblock.h"

#include <cassert>
#include <iostream>

using namespace TosLang::BackEnd;

static const std::string GetOpCodeName(const MachineInstruction::Opcode& opcode);

MachineInstruction& MachineInstruction::AddImmOperand(unsigned op)
{
    mOperands[mNumOperands++] = MachineOperand(op, MachineOperand::OperandKind::IMMEDIATE);
    return *this;
}

MachineInstruction& MachineInstruction::AddStackSlotOperand(unsigned op)
{
    mOperands[mNumOperands++] = MachineOperand(op, MachineOperand::OperandKind::STACK_SLOT);
    return *this;
}

MachineInstruction& MachineInstruction::AddRegOperand(unsigned op)
{
    mOperands[mNumOperands++] = MachineOperand(op, MachineOperand::OperandKind::REGISTER);
    return *this;
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const MachineInstruction& inst)
{
    stream << GetOpCodeName(inst.mOpCode) << " ";
    for (size_t iOp = 0; iOp < inst.mNumOperands; ++iOp)
        stream << inst.mOperands[iOp] << " ";

    return stream;
}

static const std::string GetOpCodeName(const MachineInstruction::Opcode& opcode)
{
    switch (opcode)
    {
    case MachineInstruction::Opcode::NO_OP:         return "NOP";
    case MachineInstruction::Opcode::JUMP:          return "JMP";
    case MachineInstruction::Opcode::CALL:          return "CALL";
    case MachineInstruction::Opcode::RET:           return "RET";
    case MachineInstruction::Opcode::LOAD_IMM:      return "LDI";
    case MachineInstruction::Opcode::LOAD_SP:       return "LDI_SP";
    case MachineInstruction::Opcode::LOAD:          return "LOAD";
    case MachineInstruction::Opcode::MOV:           return "MOV";
    case MachineInstruction::Opcode::STORE:         return "STM";
    case MachineInstruction::Opcode::PUSH:          return "PUSH";
    case MachineInstruction::Opcode::POP:           return "POP";
    case MachineInstruction::Opcode::ADD_IMM:       return "ADDI";
    case MachineInstruction::Opcode::ADD:           return "ADD";
    case MachineInstruction::Opcode::SUB_IMM:       return "SUBI";
    case MachineInstruction::Opcode::SUB:           return "SUB";
    case MachineInstruction::Opcode::LT:            return "LT";
    case MachineInstruction::Opcode::GT:            return "GT";
    case MachineInstruction::Opcode::AND_IMM:       return "ANDI";
    case MachineInstruction::Opcode::AND:           return "AND";
    case MachineInstruction::Opcode::OR_IMM:        return "ORI";
    case MachineInstruction::Opcode::OR:            return "OR";
    case MachineInstruction::Opcode::XOR_IMM:       return "XORI";
    case MachineInstruction::Opcode::XOR:           return "XOR";
    case MachineInstruction::Opcode::MUL_IMM:       return "MULI";
    case MachineInstruction::Opcode::MUL:           return "MUL";
    case MachineInstruction::Opcode::DIV_IMM:       return "DIVI";
    case MachineInstruction::Opcode::DIV:           return "DIV";
    case MachineInstruction::Opcode::LSHIFT:        return "LSHIFT";
    case MachineInstruction::Opcode::RSHIFT:        return "RSHIFT";
    case MachineInstruction::Opcode::MOD_IMM:       return "MODI";
    case MachineInstruction::Opcode::MOD:           return "MOD";
    case MachineInstruction::Opcode::NOT_IMM:       return "NOTI";
    case MachineInstruction::Opcode::NOT:           return "NOT";
    case MachineInstruction::Opcode::NEG_IMM:       return "NEGI";
    case MachineInstruction::Opcode::NEG:           return "NEG";
    case MachineInstruction::Opcode::UNKNOWN:       return "UNKNOWN";
    default:                                        return "";       
    }
}
