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

