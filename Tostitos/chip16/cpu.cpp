#include "cpu.h"

#include <functional>
#include <iostream>
#include <iterator>
#include <limits>

#ifdef __unix__ 
#include <string.h>
#endif

CPU::CPU() : m_FR(0), m_PC(0), m_SP(0), m_ErrorCode(NoError)
{
	memset(m_Registers, 0, sizeof(UInt16)*16);
}

UInt16 CPU::DumpFlagRegister() const
{
	return m_FR;
}

std::vector<UInt8> CPU::DumpMemory() const
{
	return std::vector<UInt8>(std::begin(*m_Memory), std::end(*m_Memory));
}

UInt16 CPU::DumpProgramCounter() const
{
	return m_PC;
}

UInt16 CPU::DumpRegister(const UInt8 in_RegID) const
{
	return m_Registers[in_RegID];	
}

std::vector<UInt16> CPU::DumpRegisters() const
{
	return std::vector<UInt16>(std::begin(m_Registers), std::end(m_Registers));
}

UInt16 CPU::DumpStackPointer() const
{
	return m_SP;
}

Instruction CPU::FetchInstruction()
{
	UInt32 l_Instruction = 0;

	for (int i = 0; i < 4; ++i)
	{
		l_Instruction <<= 8;
		l_Instruction |= (*m_Memory)[m_PC++];
	}

	return Instruction(l_Instruction);
}

void CPU::InitPC(UInt8 in_PCStart)
{
	m_PC = in_PCStart;
}

void CPU::Reset()
{
	m_ErrorCode = 0;
	m_FR = 0;
	m_SP = 0;
	m_PC = 0;
}

void CPU::SetFlagRegister(const UInt16 in_Value)
{
	// Discarding unused bits
	m_FR = in_Value & 0xFF;
}

void CPU::SetFlag(const UInt16 in_Value)
{
	m_FR |= in_Value;
}

void CPU::UnsetFlag(const UInt16 in_Value)
{
	m_FR &= ~in_Value;
}

UInt8 CPU::SetProgramCounter(const UInt16 in_Value)
{
	if(in_Value > STACK_START)
	{
		std::cout << "Not a valid value for the PC: " << std::hex << in_Value << std::endl;
		return MemoryError;
	}
	else
	{
		m_PC = in_Value;
		return NoError;
	}
}

UInt8 CPU::SetRegister(const UInt8 in_RegID, const UInt16 in_Value)
{
	if(in_RegID > NB_REGISTERS)
	{
		std::cout << "Not a valid register ID: " << std::hex << in_RegID << std::endl;
		return UnknownRegister;
	}
	else
	{
		m_Registers[in_RegID] = in_Value;
		return NoError;
	}
}

UInt8 CPU::SetStackPointer(const UInt16 in_Value)
{
	
	if(in_Value < STACK_START || in_Value > STACK_END)
	{
		std::cout << "Not a valid value for the SP: " << std::hex << in_Value << std::endl;
		return MemoryError;
	}
	else
	{
		m_SP = in_Value;
		return NoError;
	}
}

void CPU::StepBack()
{
	m_PC -= 4;
}

std::vector<UInt8> CPU::FetchPalette(const UInt16 in_Address)
{
	std::vector<UInt8> l_PaletteData(16*3);	// Per specifications

	for(int j = 0; j < 16*3; j+=3)
	{
		l_PaletteData[j+2] = (*m_Memory)[in_Address+j];
		l_PaletteData[j+1] = (*m_Memory)[in_Address+j+1];
		l_PaletteData[j] = (*m_Memory)[in_Address+j+2];
	}

	return l_PaletteData;
}

void CPU::FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y) const
{
	out_X = m_Registers[(*m_Memory)[m_PC] & 0xF];
	out_Y = m_Registers[((*m_Memory)[m_PC] & 0xF0) >> 4];
}

std::vector<UInt8> CPU::FetchSprite(const UInt16 in_Addr, const UInt16 in_Width, const UInt16 in_Height) const
{
	std::vector<UInt8> l_SpriteData(in_Width * in_Height);

	for(int i = 0; i < (in_Width * in_Height)/2; ++i)
	{
		l_SpriteData[2*i] = (*m_Memory)[in_Addr + i] >> 4;
		l_SpriteData[2*i+1] = (*m_Memory)[in_Addr + i] & 0xF;
	}

	return l_SpriteData;
}

UInt8 CPU::Load(const UInt16 in_Address, UInt16 & out_Value) const
{
	if(STACK_START < in_Address && in_Address < STACK_END)
	{
		std::cout << "Address out of memory: " << std::hex << in_Address << std::endl;
		return MemoryError;
	}
	else
	{
		out_Value = ((*m_Memory)[in_Address+1] << 8) | (*m_Memory)[in_Address];
		return NoError;
	}
}

UInt8 CPU::Store(const UInt16 in_Address, const UInt16 in_Value)
{
	if(in_Address > STACK_START)
	{
		std::cout << "Address out of memory: " << std::hex << in_Address << std::endl;
		return MemoryError;
	}
	else
	{
		(*m_Memory)[in_Address] = in_Value & 0x00FF;
		(*m_Memory)[in_Address+1] = in_Value >> 8;
		return NoError;
	}
}

UInt8 CPU::Pop(UInt16 & out_Value)
{
	if(m_SP-2 < STACK_START)
	{
		std::cout << "Stack underflow" << std::endl;
		return StackUnderflow;
	}
	else
	{
		out_Value = UInt16((*m_Memory)[--m_SP] << 8 | ((*m_Memory)[--m_SP]));
		return NoError;
	}
}

UInt8 CPU::Push(UInt16 in_Val)
{
	if(m_SP+2 > STACK_END)
	{
		std::cout << "Stack overflow while pushing: " << in_Val << std::endl;
		return StackOverflow;
	}
	else
	{
		(*m_Memory)[m_SP++] = in_Val & 0x00FF;
		(*m_Memory)[m_SP++] = (in_Val & 0xFF00) >> 8;
		return NoError;
	}
}

UInt8 CPU::PushPC()
{
	return Push(m_PC);
}

void CPU::SetSignZeroFlag(UInt16 in_Result)
{
	// Set the zero flag (Bit[2])
	m_FR = in_Result == 0 ? m_FR | ZeroFlag : m_FR & ~ZeroFlag;
	// Set the negative flag (Bit[7])
	m_FR = in_Result & 0x8000 ? m_FR | NegativeFlag : m_FR & ~NegativeFlag;
}

void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2) { }

void CPU::SetCarryOverflowFlagAdd(UInt16 in_Op1, UInt16 in_Op2) 
{
	UInt16 l_Result = in_Op1 + in_Op2;
	// Set carry flag
	m_FR = l_Result < in_Op1 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 & 0x8000 && in_Op2 & 0x8000)
		|| (l_Result & 0x8000 && in_Op1 >= 0 && in_Op2 >= 0) ?
		m_FR | SignedOverflowFlag : m_FR & ~SignedOverflowFlag;
}

void CPU::SetCarryOverflowFlagDiv(UInt16 in_Op1, UInt16 in_Op2) 
{
	// Set carry flag
	m_FR = in_Op1 % in_Op2 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::SetCarryOverflowFlagMul(UInt16 in_Op1, UInt16 in_Op2) 
{
	// Set carry flag
	UInt32 l_Result = in_Op1 * in_Op2;
	m_FR = l_Result > std::numeric_limits<UInt16>::max() ? 
		m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::SetCarryOverflowFlagSub(UInt16 in_Op1, UInt16 in_Op2) 
{
	UInt32 l_Result = in_Op1 - in_Op2;
	// Set carry flag
	m_FR = l_Result & 0x10000 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 & 0x8000 && in_Op2 >= 0)
		|| (l_Result & 0x8000 && in_Op1 >= 0 && in_Op2 & 0x8000) ?
		m_FR | SignedOverflowFlag : m_FR & ~SignedOverflowFlag;
}
