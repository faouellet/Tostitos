#include "virtualinstruction.h"

#include "basicblock.h"

#include <cassert>
#include <iostream>

using namespace TosLang::BackEnd;

static const std::string GetOpCodeName(const Instruction::InstructionOpCode& opcode);

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

static const std::string GetOpCodeName(const Instruction::InstructionOpCode& opcode)
{
    switch (opcode)
    {
    case Instruction::InstructionOpCode::NO_OP:         return "NOP";
    case Instruction::InstructionOpCode::JUMP:          return "JMP";
    case Instruction::InstructionOpCode::CALL:          return "CALL";
    case Instruction::InstructionOpCode::RET:           return "RET";
    case Instruction::InstructionOpCode::LOAD_IMM:      return "LDI";
    case Instruction::InstructionOpCode::MOV:           return "MOV";
    case Instruction::InstructionOpCode::STORE:         return "STM";
    case Instruction::InstructionOpCode::PUSH:          return "PUSH";
    case Instruction::InstructionOpCode::POP:           return "POP";
    case Instruction::InstructionOpCode::PUSHALL:       return "PUSHALL";
    case Instruction::InstructionOpCode::POPALL:        return "POPALL";
    case Instruction::InstructionOpCode::ADD_IMM:       return "ADDI";
    case Instruction::InstructionOpCode::ADD:           return "ADD";
    case Instruction::InstructionOpCode::SUB_IMM:       return "SUBI";
    case Instruction::InstructionOpCode::SUB:           return "SUB";
    case Instruction::InstructionOpCode::CMP_IMM:       return "CMPI";
    case Instruction::InstructionOpCode::CMP:           return "CMP";
    case Instruction::InstructionOpCode::AND_IMM:       return "ANDI";
    case Instruction::InstructionOpCode::AND:           return "AND";
    case Instruction::InstructionOpCode::TST:           return "TST";
    case Instruction::InstructionOpCode::OR_IMM:        return "ORI";
    case Instruction::InstructionOpCode::OR:            return "OR";
    case Instruction::InstructionOpCode::XOR_IMM:       return "XORI";
    case Instruction::InstructionOpCode::XOR:           return "XOR";
    case Instruction::InstructionOpCode::MUL_IMM:       return "MULI";
    case Instruction::InstructionOpCode::MUL:           return "MUL";
    case Instruction::InstructionOpCode::DIV_IMM:       return "DIVI";
    case Instruction::InstructionOpCode::DIV:           return "DIV";
    case Instruction::InstructionOpCode::SHIFT:         return "SHIFT";
    case Instruction::InstructionOpCode::MOD_IMM:       return "MODI";
    case Instruction::InstructionOpCode::MOD:           return "MOD";
    case Instruction::InstructionOpCode::NOT_IMM:       return "NOTI";
    case Instruction::InstructionOpCode::NOT:           return "NOT";
    case Instruction::InstructionOpCode::NEG_IMM:       return "NEGI";
    case Instruction::InstructionOpCode::NEG:           return "NEG";
    case Instruction::InstructionOpCode::UNKNOWN:       return "UNKNOWN";
    default:
        break;
    }
}
