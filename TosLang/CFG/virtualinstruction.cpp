#include "virtualinstruction.h"

#include "basicblock.h"

#include <cassert>
#include <iostream>

using namespace TosLang::BackEnd;

static const std::string GetOpCodeName(const VirtualInstruction::Opcode& opcode);

//////////////////// VirtualOperand ////////////////////

VirtualOperand::VirtualOperand(unsigned op, VirtualOperand::OperandKind kind)
{
    assert((kind == VirtualOperand::OperandKind::IMMEDIATE) 
            || (kind == VirtualOperand::OperandKind::MEM_SLOT)
            || (kind == VirtualOperand::OperandKind::REGISTER));
    mKind = kind;

    if (kind == VirtualOperand::OperandKind::IMMEDIATE)
    {
        mOperand.imm = op;
    }
    else if (kind == VirtualOperand::OperandKind::REGISTER)
    {
        mOperand.reg = op;
    }
}

VirtualOperand::VirtualOperand(BasicBlock* bb) : mKind{ VirtualOperand::OperandKind::TARGET }
{
    mOperand.target = bb;
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const VirtualOperand& op)
{
    switch (op.mKind)
    {
    case VirtualOperand::OperandKind::IMMEDIATE:
        return stream << op.mOperand.imm;
    case VirtualOperand::OperandKind::MEM_SLOT:
        return stream << "M" << op.mOperand.memslot;
    case VirtualOperand::OperandKind::REGISTER:
        return stream << "R" << op.mOperand.reg;
    case VirtualOperand::OperandKind::TARGET:
        return stream << op.mOperand.target->GetName();
    default:
        return stream;
    }
}

//////////////////// VirtualInstruction ////////////////////

VirtualInstruction& VirtualInstruction::AddImmOperand(unsigned op)
{
    mOperands[mNumOperands++] = VirtualOperand(op, VirtualOperand::OperandKind::IMMEDIATE);
    return *this;
}

VirtualInstruction& VirtualInstruction::AddMemSlotOperand(unsigned op)
{
    mOperands[mNumOperands++] = VirtualOperand(op, VirtualOperand::OperandKind::MEM_SLOT);
    return *this;
}

VirtualInstruction& VirtualInstruction::AddRegOperand(unsigned op)
{
    mOperands[mNumOperands++] = VirtualOperand(op, VirtualOperand::OperandKind::REGISTER);
    return *this;
}

VirtualInstruction& VirtualInstruction::AddTargetOperand(BasicBlock* bb)
{
    mOperands[mNumOperands++] = VirtualOperand(bb);
    return *this;
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const VirtualInstruction& inst)
{
    stream << GetOpCodeName(inst.mOpCode) << " ";
    for (size_t iOp = 0; iOp < inst.mNumOperands; ++iOp)
        stream << inst.mOperands[iOp] << " ";

    return stream;
}

static const std::string GetOpCodeName(const VirtualInstruction::Opcode& opcode)
{
    switch (opcode)
    {
    case VirtualInstruction::Opcode::NO_OP:         return "NOP";
    case VirtualInstruction::Opcode::ALLOCA:        return "ALLOCA";
    case VirtualInstruction::Opcode::JUMP:          return "JMP";
    case VirtualInstruction::Opcode::CALL:          return "CALL";
    case VirtualInstruction::Opcode::RET:           return "RET";
    case VirtualInstruction::Opcode::LOAD_IMM:      return "LDI";
    case VirtualInstruction::Opcode::MOV:           return "MOV";
    case VirtualInstruction::Opcode::STORE:         return "STM";
    case VirtualInstruction::Opcode::PUSH:          return "PUSH";
    case VirtualInstruction::Opcode::POP:           return "POP";
    case VirtualInstruction::Opcode::ADD_IMM:       return "ADDI";
    case VirtualInstruction::Opcode::ADD:           return "ADD";
    case VirtualInstruction::Opcode::SUB_IMM:       return "SUBI";
    case VirtualInstruction::Opcode::SUB:           return "SUB";
    case VirtualInstruction::Opcode::LT:            return "LT";
    case VirtualInstruction::Opcode::GT:            return "GT";
    case VirtualInstruction::Opcode::AND_IMM:       return "ANDI";
    case VirtualInstruction::Opcode::AND:           return "AND";
    case VirtualInstruction::Opcode::OR_IMM:        return "ORI";
    case VirtualInstruction::Opcode::OR:            return "OR";
    case VirtualInstruction::Opcode::XOR_IMM:       return "XORI";
    case VirtualInstruction::Opcode::XOR:           return "XOR";
    case VirtualInstruction::Opcode::MUL_IMM:       return "MULI";
    case VirtualInstruction::Opcode::MUL:           return "MUL";
    case VirtualInstruction::Opcode::DIV_IMM:       return "DIVI";
    case VirtualInstruction::Opcode::DIV:           return "DIV";
    case VirtualInstruction::Opcode::LSHIFT:        return "LSHIFT";
    case VirtualInstruction::Opcode::RSHIFT:        return "RSHIFT";
    case VirtualInstruction::Opcode::MOD_IMM:       return "MODI";
    case VirtualInstruction::Opcode::MOD:           return "MOD";
    case VirtualInstruction::Opcode::NOT_IMM:       return "NOTI";
    case VirtualInstruction::Opcode::NOT:           return "NOT";
    case VirtualInstruction::Opcode::NEG_IMM:       return "NEGI";
    case VirtualInstruction::Opcode::NEG:           return "NEG";
    case VirtualInstruction::Opcode::UNKNOWN:       return "UNKNOWN";
    default:                                        return "";       
    }
}
