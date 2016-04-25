#include "machineoperand.h"

#include "basicblock.h"

#include <cassert>
#include <iostream>
#include <new>

using namespace TosLang::BackEnd;

MachineOperand::MachineOperand() : mKind{ OperandKind::UNKNOWN } { }

MachineOperand::MachineOperand(const unsigned op, const OperandKind kind)
{
    assert((kind == OperandKind::IMMEDIATE) 
            || (kind == OperandKind::STACK_SLOT)
            || (kind == OperandKind::REGISTER));
    mKind = kind;

    switch (kind)
    {
    case OperandKind::IMMEDIATE:
        imm = op;
        break;
    case OperandKind::STACK_SLOT:
        stackslot = op;
        break;
    case OperandKind::REGISTER:
        reg = op;
        break;
    default:
        assert(false && "Unexpected error while building a virtual instruction");
        break;
    }
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const MachineOperand& op)
{
    switch (op.mKind)
    {
    case MachineOperand::OperandKind::IMMEDIATE:
        return stream << op.imm;
    case MachineOperand::OperandKind::STACK_SLOT:
        return stream << "S" << op.stackslot;
    case MachineOperand::OperandKind::REGISTER:
        return stream << "R" << op.reg;
    default:
        return stream;
    }
}
