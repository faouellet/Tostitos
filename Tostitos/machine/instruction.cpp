#include "instruction.h"

#include <cstring>

using namespace MachineEngine::ProcessorSpace;

Instruction::Instruction(const Utils::UInt32 value) : mValue{ value } { }

Instruction::Instruction(Instruction::InstructionOpCode iType)
{
    mValue = static_cast<UInt32>(iType) << 24;
}

UInt8 Instruction::GetOpcode() const
{
    return mValue >> 24;
}

UInt8 Instruction::GetFirstOperand() const
{
    return (mValue >> 16) & 0xF;
}

UInt8 Instruction::GetSecondOperand() const
{
    return (mValue >> 20) & 0xF;
}

UInt8 Instruction::GetThirdOperand() const
{
    return (mValue >> 8) & 0xF;
}

UInt16 Instruction::GetImmediateValue() const
{
    return ((mValue << 8) & 0xFF00) | ((mValue >> 8) & 0xFF);
}

UInt8 Instruction::GetType() const
{
    return static_cast<InstructionOpCode>(mValue >> 24);
}

// TODO: The setters are good for now, but they might need to change in the long term. I see 2 possibles options:
//       1- Find a way to set only part of an integer
//       2- Restrict the use of these functions to instructions not already having their operands set

void Instruction::SetFirstOperand(UInt8 value)
{
    mValue |= static_cast<UInt32>(value) << 16;

    // mValue &= static_cast<UInt32>(value) << 16;
}

void Instruction::SetSecondOperand(UInt8 value)
{
    if (!UseImmediateValue())
    {
        mValue |= static_cast<UInt32>(value) << 20;
        // mValue &= static_cast<UInt32>(value) << 20;
    }
}

void Instruction::SetThirdOperand(UInt8 value)
{
    if (!UseImmediateValue() && !IsInplace())
    {
        mValue |= static_cast<UInt32>(value) << 8;
        // mValue &= static_cast<UInt32>(value) << 8;
    }
}

void Instruction::SetImmediateValue(UInt16 value)
{
    if (UseImmediateValue())
    {
        mValue |= (((value << 8) & 0xFF00) | ((value >> 8) & 0xFF));
        // mValue &= (((value << 8) & 0xFF00) & ((value >> 8) & 0xFF));
    }
}

// TODO: Lack of precondition checking UseImmediateValue and IsInplace

bool Instruction::UseImmediateValue() const
{
    return !(mValue & 0x0F000000);
}

bool Instruction::IsArithmeticInstruction() const
{
    return mValue >= 0x05000000;
}

bool Instruction::IsInplace() const
{
    return (mValue & 0x01000000) != 0;
}

// TODO: Find a better way to test for second operand

bool Instruction::UseSecondOperand() const
{
    switch (GetOpcode())
    {
    case 0x13:
    case 0x23:
    case 0x24:
    case 0x31:
    case 0x51:
    case 0x52:
    case 0x61:
    case 0x62:
    case 0x64:
    case 0x71:
    case 0x72:
    case 0x74:
    case 0x81:
    case 0x82:
    case 0x91:
    case 0x92:
    case 0xA1:
    case 0xA2:
    case 0xB1:
    case 0xB2:
    case 0xC3:
    case 0xC4:
    case 0xC5:
    case 0xD1:
    case 0xD2:
    case 0xE1:
    case 0xE2:
    case 0xF1:
    case 0xF2:
        return true;
    default:
        return false;
    }
}

bool Instruction::UseThirdOperand() const
{
    return IsArithmeticInstruction() && (((mValue >> 24) & 0x0F) == 2);
}
