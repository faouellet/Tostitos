#include "instruction.h"

using namespace MachineEngine::ProcessorSpace;

Instruction::Instruction(const Utils::UInt32 in_Value) : m_Value{in_Value}, m_IsInplace{false}, m_UseImm{ false }
{
    m_Opcode = m_Value >> 24;
    m_Type = m_Opcode & 0xF0;
    m_Op1 = FetchHalfByte(4);

    if(Store < m_Type && m_Type < Stack)	// Arithmetic instructions
    {
        if(m_Type == Shift)
        {
            m_UseImm = (m_Opcode & 0xF) < 3;
        
            if(m_UseImm)
                m_Op3 = FetchHalfByte(2);
            else
                m_Op2 = FetchHalfByte(5);
        }
        else
        {
            m_UseImm = ((m_Opcode & 0xF) == 0) || ((m_Opcode & 0xF) == 3);
        
            if(m_UseImm)
            {
                UInt16 l_IVal = (m_Value >> 8) & 0xFF;
                m_ImmediateValue = l_IVal | ((m_Value & 0xFF) << 8);
            }
            else
            {
                m_Op2 = FetchHalfByte(5);
        
                if ((m_Opcode & 0xF) == 2 || ((m_Opcode & 0xF) == 5 && m_Type == Div))
                {
                    m_IsInplace = false;
                    m_Op3 = FetchHalfByte(2);
                }
                else
                {
                    m_IsInplace = true;
                }
            }
        }
    }
    else
    {
        m_Op2 = FetchHalfByte(5);
        m_Op3 = FetchHalfByte(2);
        UInt16 l_IVal = (m_Value >> 8) & 0xFF;
        m_ImmediateValue = l_IVal | ((m_Value & 0xFF) << 8);
    }
}

UInt8 Instruction::FetchHalfByte(const UInt8 in_Pos) const
{
    return (m_Value >> (in_Pos*4)) & 0xF;
}

UInt8 Instruction::GetOpcode() const
{
    return m_Opcode;
}

UInt8 Instruction::GetFirstOperand() const
{
    return m_Op1;
}

UInt8 Instruction::GetSecondOperand() const
{
    return m_Op2;
}

UInt8 Instruction::GetThirdOperand() const
{
    return m_Op3;
}

UInt16 Instruction::GetImmediateValue() const
{
    return m_ImmediateValue;
}

UInt8 Instruction::GetType() const
{
    return m_Type;
}

bool Instruction::UseImmediateValue() const
{
    return m_UseImm;
}

bool Instruction::IsInplace() const
{
    return m_IsInplace;
}
