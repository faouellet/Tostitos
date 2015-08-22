#include "instruction.h"

using namespace MachineEngine::ProcessorSpace;

Instruction::Instruction(const Utils::UInt32 value) : mValue{ value }, mIsInplace{ false }, mUseImm{ false }
{
    mOpcode = mValue >> 24;
    mType = mOpcode & 0xF0;
    mOp1 = FetchHalfByte(4);

    if(STORE < mType && mType < STACK)	// Arithmetic instructions
    {
        if(mType == SHIFT)
        {
            mUseImm = (mOpcode & 0xF) < 3;
        
            if(mUseImm)
                mOp3 = FetchHalfByte(2);
            else
                mOp2 = FetchHalfByte(5);
        }
        else
        {
            mUseImm = ((mOpcode & 0xF) == 0) || ((mOpcode & 0xF) == 3);
        
            if(mUseImm)
            {
                UInt16 iVal = (mValue >> 8) & 0xFF;
                mImmediateValue = iVal | ((mValue & 0xFF) << 8);
            }
            else
            {
                mOp2 = FetchHalfByte(5);
        
                if ((mOpcode & 0xF) == 2 || ((mOpcode & 0xF) == 5 && mType == DIV_IMM))
                {
                    mIsInplace = false;
                    mOp3 = FetchHalfByte(2);
                }
                else
                {
                    mIsInplace = true;
                }
            }
        }
    }
    else
    {
        mOp2 = FetchHalfByte(5);
        mOp3 = FetchHalfByte(2);
        UInt16 iVal = (mValue >> 8) & 0xFF;
        mImmediateValue = iVal | ((mValue & 0xFF) << 8);
    }
}

UInt8 Instruction::FetchHalfByte(const UInt8 pos) const
{
    return (mValue >> (pos * 4)) & 0xF;
}

UInt8 Instruction::GetOpcode() const
{
    return mOpcode;
}

UInt8 Instruction::GetFirstOperand() const
{
    return mOp1;
}

UInt8 Instruction::GetSecondOperand() const
{
    return mOp2;
}

UInt8 Instruction::GetThirdOperand() const
{
    return mOp3;
}

UInt16 Instruction::GetImmediateValue() const
{
    return mImmediateValue;
}

UInt8 Instruction::GetType() const
{
    return mType;
}

bool Instruction::UseImmediateValue() const
{
    return mUseImm;
}

bool Instruction::IsInplace() const
{
    return mIsInplace;
}
