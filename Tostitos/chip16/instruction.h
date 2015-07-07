#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../Utils/utils.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/**
* \class Instruction
* \brief Abstraction over a Chip16 instruction
*/
class Instruction
{
private:
	UInt32 m_Value;		/*!< 4 byte value of the instruction */
	
	UInt16 m_ImmediateValue;	/*!< Last 2 bytes interpreted as an immediate value */
	
	UInt8 m_Op1;		/*!< 1st operand contained in the instruction */
	UInt8 m_Op2;		/*!< 2nd operand contained in the instruction */
	UInt8 m_Op3;		/*!< 3rd operand contained in the instruction */
	UInt8 m_Opcode;		/*!< Opcode of the instruction */
	UInt8 m_Type;		/*!< Type of the instruction */

	bool m_UseImm;		/*!< Does the instruction use an immediate value? */
	bool m_IsInplace;	/*!< Is the instruction done inplace? */

public:
	/**
	* \enum
	* \brief Leading byte for the different types of instruction
	*/
	enum 
	{
		Misc  = 0x00, 
		Jump  = 0x10, 
		Load  = 0x20, 
		Store = 0x30, 
		Add   = 0x40,
		Sub   = 0x50,
		And   = 0x60,
		Or    = 0x70,
		Xor   = 0x80,
		Mul   = 0x90,
		Div   = 0xA0,
		Shift = 0xB0,
		Stack = 0xC0,
		Pal   = 0xD0
	};

public:
	/**
	* \fn Instruction
	* \brief Default constructor
	* \param in_Value Value of the instruction. 0 by default.
	*/
	Instruction(const UInt32 in_Value = 0);
	
	/**
	* \fn Instruction
	* \brief Destructor
	*/
	~Instruction();

private:
	/**
	* \fn FetchHalfByte
	* \brief Fetch half of an instruction byte
	* \param in_Pos 0-based position of the half-byte within the instruction starting from the right
	* \return A 4 bits value
	*/
	UInt8 FetchHalfByte(const UInt8 in_Pos) const;

public:
	/**
	* \fn GetOpcode
	* \brief Fetch the instruction opcode
	* \return The instruction opcode
	*/
	UInt8 GetOpcode() const;

	/**
	* \fn GetFirstOperand
	* \brief Fetch the first operand of the instruction
	* \return The first operand of the instruction
	*/
	UInt8 GetFirstOperand() const;
	
	/**
	* \fn GetSecondOperand
	* \brief Fetch the second operand of the instruction when the instruction 
	*        doesn't use an immediate value
	* \return The second operand of the instruction or pure garbage
	*/
	UInt8 GetSecondOperand() const;
	
	/**
	* \fn GetThirdOperand
	* \brief Fetch the third operand of the instruction when the instruction 
	*        doesn't use an immediate value or isn't inplace
	* \return The third operand of the instruction or pure garbage
	*/
	UInt8 GetThirdOperand() const;
	
	/**
	* \fn GetImmediateValue
	* \brief Fetch the immediate value contained in the instruction
	* \return The immediate value contained in the instruction
	*/
	UInt16 GetImmediateValue() const;
	
	/**
	* \fn GetType
	* \brief Fetch the type of the instruction
	* \return The type of the instruction
	*/
	UInt8 GetType() const;

public:
	/**
	* \fn UseImmediateValue
	* \brief Indicate if the instruction make use of an immediate value
	* \return Boolean indicating the use of an immediate value
	*/
	bool UseImmediateValue() const;
	
	/**
	* \fn IsInplace
	* \brief Indicate if the instruction is an inplace operation
	* \return Boolean indicating if the instruction us an inplace operation
	*/
	bool IsInplace() const;
};

#endif // INSTRUCTION_H
