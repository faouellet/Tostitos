#include "virtualinstruction.h"

#include "basicblock.h"

#include <cassert>

using namespace TosLang::BackEnd;

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

std::ostream & TosLang::BackEnd::operator<<(std::ostream & stream, const VirtualOperand & op)
{
    switch (op.mKind)
    {
    case VirtualOperand::OperandKind::IMMEDIATE:
        stream << op.mOperand.imm;
        break;
    case VirtualOperand::OperandKind::REGISTER:
        stream << op.mOperand.reg;
        break;
    case VirtualOperand::OperandKind::TARGET:
        stream << op.mOperand.target->GetName();
        break;
    default:
        break;
    }

    return stream;
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

std::ostream& TosLang::BackEnd::operator<<(std::ostream & stream, const VirtualInstruction & inst)
{
    stream << inst.mOpCode << " ";
    for (size_t iOp = 0; iOp < inst.mNumOperands; ++iOp)
        stream << inst.mOperands[iOp] << " ";

    return stream;
}
