#include "cpu.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>

#ifdef __unix__ 
#include <string.h>
#endif

using namespace MachineEngine::ProcessorSpace;
using MachineEngine::ProcessorSpace::Utils::Int16;

CPU::CPU() : mFR{ 0 }, mPC{ 0 }, mSP{ STACK_START }, mErrorCode{ NO_ERROR }
{
    memset(mRegisters, 0, sizeof(UInt16)*16);
    
}

UInt16 CPU::DumpFlagRegister() const
{
    return mFR;
}

std::vector<UInt8> CPU::DumpMemory() const
{
    return std::vector<UInt8>(std::begin(mMemory), std::end(mMemory));
}

UInt16 CPU::DumpProgramCounter() const
{
    return mPC;
}

UInt16 CPU::DumpRegister(const UInt8 in_RegID) const
{
    return mRegisters[in_RegID];
}

std::vector<UInt16> CPU::DumpRegisters() const
{
    return std::vector<UInt16>(std::begin(mRegisters), std::end(mRegisters));
}

UInt16 CPU::DumpStackPointer() const
{
    return mSP;
}

Instruction CPU::FetchInstruction()
{
    UInt32 instruction = 0;

    for (int i = 0; i < 4; ++i)
    {
        instruction <<= 8;
        instruction |= mMemory[mPC++];
    }

    return Instruction(instruction);
}

unsigned CPU::InitMemory(std::vector<UInt8> && program)
{
    if (program.empty())
        return EMPTY_ROM_ERROR;
    else if (program.size() > MEMORY_SIZE)
        return ROM_OVERFLOW_ERROR;

    std::copy_n(std::make_move_iterator(program.begin()), program.size(), mMemory.begin());

    return NO_ERROR;
}

void CPU::Reset()
{
    mErrorCode = 0;
    mFR = 0;
    mSP = 0;
    mPC = 0;
}

void CPU::SetFlagRegister(const UInt16 value)
{
    // Discarding unused bits
    mFR = value & 0xFF;
}

UInt8 CPU::SetProgramCounter(const UInt16 value)
{
    if(value > STACK_START)
    {
		PrintHex("Not a valid value for the PC", value);
        return MEMORY_ERROR;
    }
    else
    {
        mPC = value;
        return NO_ERROR;
    }
}

UInt8 CPU::SetRegister(const UInt8 regID, const UInt16 value)
{
    if(regID >= NB_REGISTERS)
    {
		PrintHex("Not a valid register ID", regID);
        return UNKNOWN_REGISTER;
    }
    else
    {
        mRegisters[regID] = value;
        return NO_ERROR;
    }
}

UInt8 CPU::SetStackPointer(const UInt16 value)
{
    
    if(value < STACK_START || value > STACK_END)
    {
        std::ios::fmtflags f(std::cout.flags());
        std::cout << "Not a valid value for the SP: " << std::hex << value << std::endl;
        std::cout.flags(f);
     
        return MEMORY_ERROR;
    }
    else
    {
        mSP = value;
        return NO_ERROR;
    }
}

UInt8 CPU::StepBack()
{
    if ((mPC == 0) || ((mPC - 4) > STACK_START))
    {
        PrintHex("Not a valid value for the PC", mPC - 4);
        return MEMORY_ERROR;
    }
    else
    {
        mPC -= 4;
        return NO_ERROR;
    }
}

void CPU::PrintHex(const std::string& message, UInt16 value) const
{
	std::ios::fmtflags f(std::cout.flags());
	std::cout << message << ": " << std::hex << value << std::endl;
	std::cout.flags(f);
}

void CPU::FetchRegistersValues(UInt16 & x, UInt16 & y) const
{
    x = mRegisters[mMemory[mPC] & 0xF];
    y = mRegisters[(mMemory[mPC] & 0xF0) >> 4];
}

UInt8 CPU::Load(const UInt16 address, UInt16 & value) const
{
    if(STACK_START < address && address < STACK_END)
    {
		PrintHex("Address out of memory", address);
        return MEMORY_ERROR;
    }
    else
    {
        value = (mMemory[address+1] << 8) | mMemory[address];
        return NO_ERROR;
    }
}

UInt8 CPU::Store(const UInt16 address, const UInt16 value)
{
    if(address > STACK_START)
    {
		PrintHex("Address out of memory", address);
        return MEMORY_ERROR;
    }
    else
    {
        mMemory[address] = value & 0x00FF;
        mMemory[address+1] = value >> 8;
        return NO_ERROR;
    }
}

UInt8 CPU::Pop(UInt16 & value)
{
    if((mSP - 2) < STACK_START)
    {
        std::cout << "Stack underflow" << std::endl;
        return STACK_UNDERFLOW;
    }
    else
    {
        const UInt8 valueLowNibble = mMemory[--mSP];
        const UInt8 valueHighNibble = mMemory[--mSP];
        value = UInt16((valueLowNibble << 8) | valueHighNibble);
        return NO_ERROR;
    }
}

UInt8 CPU::Push(UInt16 val)
{
    if(mSP + 2 > STACK_END)
    {
        std::cout << "Stack overflow while pushing: " << val << std::endl;
        return STACK_OVERFLOW;
    }
    else
    {
        mMemory[mSP++] = val & 0x00FF;
        mMemory[mSP++] = (val & 0xFF00) >> 8;
        return NO_ERROR;
    }
}

UInt8 CPU::PushPC()
{
    return Push(mPC);
}

void CPU::SetSignZeroFlag(UInt16 result)
{
    // Set the zero flag (Bit[2])
    mFR = result == 0 ? mFR | ZERO_FLAG : mFR & ~ZERO_FLAG;
    // Set the negative flag (Bit[7])
    mFR = result & 0x8000 ? mFR | NEGATIVE_FLAG : mFR & ~NEGATIVE_FLAG;
}

void CPU::SetCarryOverflowFlagAdd(UInt16 op1, UInt16 op2) 
{
	// TODO: Simplify this function. The conditions are too error-prone.

    UInt16 result = op1 + op2;
    // Set carry flag
    mFR = result < op1 ? mFR | UNSIGNED_CARRY_FLAG : mFR & ~UNSIGNED_CARRY_FLAG;

	Int16 op1Signed = static_cast<Int16>(op1);
	Int16 op2Signed = static_cast<Int16>(op2);

    // Set overflow flag
    mFR = ((op1Signed & 0x8000) && (op2Signed & 0x8000))
        || ((result & 0x8000) && (op1Signed >= 0) && (op2Signed >= 0)) ?
        mFR | SIGNED_OVERFLOW_FLAG : mFR & ~SIGNED_OVERFLOW_FLAG;
}

void CPU::SetCarryOverflowFlagDiv(UInt16 op1, UInt16 op2) 
{
    // Set carry flag
    mFR = op1 % op2 ? mFR | UNSIGNED_CARRY_FLAG : mFR & ~UNSIGNED_CARRY_FLAG;
}

void CPU::SetCarryOverflowFlagMul(UInt16 op1, UInt16 op2) 
{
    // Set carry flag
    UInt32 result = op1 * op2;
    mFR = result > std::numeric_limits<UInt16>::max() ? 
        mFR | UNSIGNED_CARRY_FLAG : mFR & ~UNSIGNED_CARRY_FLAG;
}

void CPU::SetCarryOverflowFlagSub(UInt16 op1, UInt16 op2) 
{
    UInt32 result = op1 - op2;
    // Set carry flag
    mFR = result & 0x10000 ? mFR | UNSIGNED_CARRY_FLAG : mFR & ~UNSIGNED_CARRY_FLAG;
    // Set overflow flag
    mFR = (op1 & 0x8000)
        || (result & 0x8000 && op2 & 0x8000) ?
        mFR | SIGNED_OVERFLOW_FLAG : mFR & ~SIGNED_OVERFLOW_FLAG;
}
