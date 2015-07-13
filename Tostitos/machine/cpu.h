#ifndef CPU_H
#define CPU_H

#include "constants.h"
#include "instruction.h"

#include <array>
#include <memory>
#include <vector>

namespace MachineEngine
{
	namespace ProcessorSpace
	{
		/**
		* \class CPU
		* \brief Emulates the Central Processing Unit (CPU) of the MinChip16 architecture
		*/
		class CPU
		{
		public:
			/**
			* \enum
			* \brief Possible interpretation errors
			*/
			enum { NoError = 0, UnknownRegister = 1, StackOverflow = 2, StackUnderflow = 4, StackError = 8, MemoryError = 16 };

			/**
			* \enum
			* \brief Masks for the flag register
			*/
			enum {
				UnsignedCarryFlag = 2, UnsignedBorrowFlag = 2, ZeroFlag = 4,
				SignedOverflowFlag = 64, NegativeFlag = 128
			};

			/**
			* \enum
			* \brief Number of registers
			*/
			enum { NB_REGISTERS = 16 };

		private:
			UInt8 m_FR;										/*!< Flag register */
			UInt16 m_PC;									/*!< Program counter */
			UInt16 m_SP;									/*!< Stack pointer */

			UInt16 m_ErrorCode;								/*!< Code used when an error happens during emulation */

			UInt16 m_Registers[NB_REGISTERS];				/*!< General purpose registers */
			std::array<UInt8, MEMORY_SIZE> m_Memory;		/*!< Memory of the CPU. See specs for layout details */

		public:
			/**
			* \fn CPU
			* \brief Default constructor
			*/
			CPU();

			/**
			* \fn ~CPU
			* \brief Destructor
			*/
			~CPU() = default;

		public:
			/**
			* \fn DumpFlagRegister
			* \brief Dump the flag register
			* \return The content of the flag register
			*/
			UInt16 DumpFlagRegister() const;

			/**
			* \fn DumpMemory
			* \brief Dump the CPU memory
			* \return The data contained in the CPU memory
			*/
			std::vector<UInt8> DumpMemory() const;

			/**
			* \fn DumpProgramCounter
			* \brief Dump the program counter
			* \return The memory address pointed by the PC
			*/
			UInt16 DumpProgramCounter() const;

			/**
			* \fn DumpRegister
			* \brief Dump the value contained in a register
			* \param in_RegID The ID of the register
			* \return The value contained in the register
			*/
			UInt16 DumpRegister(const UInt8 in_RegID) const;

			/**
			* \fn DumpRegisters
			* \brief Dump the CPU registers
			* \return The data contained in the CPU registers
			*/
			std::vector<UInt16> DumpRegisters() const;

			/**
			* \fn DumpStackPointer
			* \brief Dump the stack pointer
			* \return The memory address pointed by the SP
			*/
			UInt16 DumpStackPointer() const;

		public:
			/**
			* \fn FetchInstruction
			* \brief Fetch a Chip16 instruction from the emulator memory
			* \return Chip16 instruction
			*/
			Instruction FetchInstruction();

			/**
			* \fn InitMemory
			* \brief Initialize the CPU with a program
			* \param in_Program Sequence of bytes representing a program
			* \return Error code depending on the situation
			*/
			unsigned InitMemory(std::vector<UInt8> && in_Program);

			/**
			* \fn InitPC
			* \brief Set the program counter at the start of the program
			* \param in_PCStart Starting address of the program counter
			*/
			void InitPC(UInt8 in_PCStart);

			/**
			* \fn Reset
			* \brief Restore the central processing unit at its pre-initialized state
			*/
			void Reset();

			/**
			* \fn SetFlagRegister
			* \brief Set the value of the flag register
			* \param in_Value The new value of the PC
			*/
			void SetFlagRegister(const UInt16 in_Value);

			/**
			* \fn SetFlag
			* \brief Set a flag within the flag register
			* \param in_Value The flag to set
			*/
			void SetFlag(const UInt16 in_Value);

			/**
			* \fn UnsetFlag
			* \brief Unset a flag within the flag register
			* \param in_Value The flag to unset
			*/
			void UnsetFlag(const UInt16 in_Value);

			/**
			* \fn SetProgramCounter
			* \brief Set the value of the PC
			* \param in_Value The new value of the PC
			* \return ErrorCode
			*/
			UInt8 SetProgramCounter(const UInt16 in_Value);

			/**
			* \fn SetRegister
			* \brief Set the value of a register
			* \param in_RegID The ID of the register to update
			* \param in_Value The new value of the register
			* \return ErrorCode
			*/
			UInt8 SetRegister(const UInt8 in_RegID, const UInt16 in_Value);

			/**
			* \fn SetStackPointer
			* \brief Set the value of the SP
			* \param in_Value The new value of the SP
			* \return ErrorCode
			*/
			UInt8 SetStackPointer(const UInt16 in_Value);

			/**
			* \fn StepBack
			* \brief Make the PC go back to the previous instruction
			*/
			void StepBack();

		public:	// Memory helpers
			/**
			* \fn FetchPalette
			* \brief Read a number of bytes from memory which corresponds to a palette data
			* \param in_Address Address in memory where the palette is stored
			* \return The palette data
			*/
			std::vector<UInt8> FetchPalette(const UInt16 in_Address);

			/**
			* \fn FetchRegistersValues
			* \brief Extract the values contained within the registers whose addresses
			*        are in the byte pointed by the PC
			*/
			void FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y) const;

			/**
			* \fn FetchSprite
			* \brief Read a number of bytes from memory which corresponds to a sprite data
			* \param in_Addr Address in memory where the sprite is stored
			* \param in_Width The horizontal dimension of the sprite
			* \param in_Height The vertical dimension of the sprite
			* \return The sprite data
			*/
			std::vector<UInt8> FetchSprite(const UInt16 in_Addr, const UInt16 in_Width, const UInt16 in_Height) const;

			/**
			* \fn Load
			* \brief Load a value from memory
			* \param in_Address Memory address of the value to fetch
			* \param out_Value The value at the given address
			* \return Error code
			*/
			UInt8 Load(const UInt16 in_Address, UInt16 & out_Value) const;

			/**
			* \fn Store
			* \brief Store a value in memory
			* \param in_Address Memory address of the value to fetch
			* \param in_Value The value to store
			* \return Error code
			*/
			UInt8 Store(const UInt16 in_Address, const UInt16 in_Value);

		public:	// Stack helpers
			/**
			* \fn Pop
			* \brief Pop a value from the stack of the emulator and decrement the SP by 2
			* \param out_Value The value that was at the top of the stack
			* \return Error code
			*/
			UInt8 Pop(UInt16 & out_Value);

			/**
			* \fn Push
			* \brief Push a value on the stack and increment the SP by 2
			* \param in_Val The value to be pushed
			* \return Error code
			*/
			UInt8 Push(UInt16 in_Val);

			/**
			* \fn PushPC
			* \brief Push the PC on the stack and increment the SP by 2
			* \return Error code
			*/
			UInt8 PushPC();

		public:	// Flag register helpers
			/**
			* \fn SetSignZeroFlag
			* \brief Set the sign flag if the bit[15] of the result is lit
			*        and set the zero flag if the result is zero
			*/
			void SetSignZeroFlag(UInt16 in_Result);

			/**
			* \fn SetCarryOverflowFlag*
			* \brief Set the carry and the overflow flag depending on the instruction used.
			* \param in_Op1 The left hand side operand in a computation
			* \param in_Op2 The right hand side operand in a computation
			*/
			void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2);
			void SetCarryOverflowFlagAdd(UInt16 in_Op1, UInt16 in_Op2);
			void SetCarryOverflowFlagDiv(UInt16 in_Op1, UInt16 in_Op2);
			void SetCarryOverflowFlagMul(UInt16 in_Op1, UInt16 in_Op2);
			void SetCarryOverflowFlagSub(UInt16 in_Op1, UInt16 in_Op2);
		};
	}
}

#endif // CPU_H
