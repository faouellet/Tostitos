#include "instruction.h"

#include <cstring>

using namespace MachineEngine::ProcessorSpace;

Instruction::Instruction(const Utils::UInt32 value) : mValue{ value } { }

Instruction::Instruction(Instruction::InstructionType iType)
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
    return static_cast<InstructionType>(mValue >> 24);
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

bool Instruction::UseImmediateValue() const
{
    return !(mValue & 0x0F000000);
}

bool Instruction::IsInplace() const
{
    return (mValue & 0x01000000);
}
