#include "virtualinstruction.h"

#include "basicblock.h"

#include <cassert>
#include <iostream>
#include <new>

using namespace TosLang::BackEnd;

VirtualOperand::VirtualOperand() : mKind{ OperandKind::UNKNOWN } { }

VirtualOperand::VirtualOperand(unsigned op, OperandKind kind)
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
        memslot = op;
        break;
    case OperandKind::REGISTER:
        reg = op;
        break;
    default:
        assert(false && "Unexpected error while building a virtual instruction");
        break;
    }
}

VirtualOperand::VirtualOperand(BasicBlock* bb) : mKind{ VirtualOperand::OperandKind::TARGET }
{
    target = bb;
}

VirtualOperand::VirtualOperand(const std::string& funcName) : mKind{ VirtualOperand::OperandKind::FUNCTION }
{
    new (&fName) std::string(funcName);
}

VirtualOperand::VirtualOperand(const VirtualOperand& op)
{
    mKind = op.mKind;
    switch (op.mKind)
    {
    case OperandKind::FUNCTION:
        new (&fName) std::string(op.fName);
        break;
    case OperandKind::IMMEDIATE:
        imm = op.imm;
        break;
    case OperandKind::REGISTER:
        reg = op.reg;
        break;
    case OperandKind::STACK_SLOT:
        memslot = op.memslot;
        break;
    case OperandKind::TARGET:
        target = op.target;
        break;
    default:
        break;
    }
}

VirtualOperand::~VirtualOperand()
{
    if (mKind == OperandKind::FUNCTION)
    {
        // The using statement is needed because of a bug in Clang 3.8.
        // This will probably be removed at a later time.
        using std::string;
        fName.~string();
    }
}

VirtualOperand& VirtualOperand::operator=(const VirtualOperand& op)
{
    // TODO: Refactor to use copy-and-swap
    if (this != &op)
    {
        mKind = op.mKind;
        switch (op.mKind)
        {
        case OperandKind::FUNCTION:
            new (&fName) std::string(op.fName);
            break;
        case OperandKind::IMMEDIATE:
            imm = op.imm;
            break;
        case OperandKind::REGISTER:
            reg = op.reg;
            break;
        case OperandKind::STACK_SLOT:
            memslot = op.memslot;
            break;
        case OperandKind::TARGET:
            target = op.target;
            break;
        default:
            break;
        }
    }
    return *this;
}

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const VirtualOperand& op)
{
    switch (op.mKind)
    {
    case VirtualOperand::OperandKind::FUNCTION:
        return stream << op.fName;
    case VirtualOperand::OperandKind::IMMEDIATE:
        return stream << op.imm;
    case VirtualOperand::OperandKind::STACK_SLOT:
        return stream << "S" << op.memslot;
    case VirtualOperand::OperandKind::REGISTER:
        return stream << "R" << op.reg;
    case VirtualOperand::OperandKind::TARGET:
        return stream << op.target->GetName();
    default:
        return stream;
    }
}
