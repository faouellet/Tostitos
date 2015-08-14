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
        private:
            friend class Interpreter;

        public:
            /**
            * \enum
            * \brief Possible interpretation errors
            */
            enum { NO_ERROR = 0, UNKNOWN_REGISTER = 1, STACK_OVERFLOW = 2, STACK_UNDERFLOW = 4, STACK_ERROR = 8, MEMORY_ERROR = 16 };

            /**
            * \enum
            * \brief Masks for the flag register
            */
            enum {
                UNSIGNED_CARRY_FLAG = 2, UNSIGNED_BORROW_FLAG = 2, ZERO_FLAG = 4,
                SIGNED_OVERFLOW_FLAG = 64, NEGATIVE_FLAG = 128
            };

            /**
            * \enum
            * \brief Number of registers
            */
            enum { NB_REGISTERS = 16 };

        private:
            UInt8 mFR;										/*!< Flag register */
            UInt16 mPC; 									/*!< Program counter */
            UInt16 mSP;	    								/*!< Stack pointer */

            UInt16 mErrorCode;								/*!< Code used when an error happens during emulation */

            UInt16 mRegisters[NB_REGISTERS];				/*!< General purpose registers */
            std::array<UInt8, MEMORY_SIZE> mMemory; 		/*!< Memory of the CPU. See specs for layout details */

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
            * \param regID The ID of the register
            * \return The value contained in the register
            */
            UInt16 DumpRegister(const UInt8 regID) const;

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
            * \param program Sequence of bytes representing a program
            * \return Error code depending on the situation
            */
            unsigned InitMemory(std::vector<UInt8> && program);

            /**
            * \fn InitPC
            * \brief Set the program counter at the start of the program
            * \param pcStart Starting address of the program counter
            */
            void InitPC(UInt8 pcStart);

            /**
            * \fn Reset
            * \brief Restore the central processing unit at its pre-initialized state
            */
            void Reset();

            /**
            * \fn SetFlagRegister
            * \brief Set the value of the flag register
            * \param value The new value of the PC
            */
            void SetFlagRegister(const UInt16 value);

            /**
            * \fn SetFlag
            * \brief Set a flag within the flag register
            * \param value The flag to set
            */
            void SetFlag(const UInt16 value);

            /**
            * \fn UnsetFlag
            * \brief Unset a flag within the flag register
            * \param value The flag to unset
            */
            void UnsetFlag(const UInt16 value);

            /**
            * \fn SetProgramCounter
            * \brief Set the value of the PC
            * \param value The new value of the PC
            * \return ErrorCode
            */
            UInt8 SetProgramCounter(const UInt16 value);

            /**
            * \fn SetRegister
            * \brief Set the value of a register
            * \param regID The ID of the register to update
            * \param value The new value of the register
            * \return ErrorCode
            */
            UInt8 SetRegister(const UInt8 regID, const UInt16 value);

            /**
            * \fn SetStackPointer
            * \brief Set the value of the SP
            * \param value The new value of the SP
            * \return ErrorCode
            */
            UInt8 SetStackPointer(const UInt16 value);

            /**
            * \fn StepBack
            * \brief Make the PC go back to the previous instruction
            */
            void StepBack();

        private:	// Memory helpers
            /**
            * \fn FetchPalette
            * \brief Read a number of bytes from memory which corresponds to a palette data
            * \param address Address in memory where the palette is stored
            * \return The palette data
            */
            std::vector<UInt8> FetchPalette(const UInt16 address);

            /**
            * \fn FetchRegistersValues
            * \brief Extract the values contained within the registers whose addresses
            *        are in the byte pointed by the PC
            * \param[out] x Left hand side operand of the instruction
            * \param[out] y Right hand side operand of the instruction
            */
            void FetchRegistersValues(UInt16 & x, UInt16 & y) const;

            /**
            * \fn Load
            * \brief Load a value from memory
            * \param address Memory address of the value to fetch
            * \param[out] value The value at the given address
            * \return Error code
            */
            UInt8 Load(const UInt16 address, UInt16 & value) const;

            /**
            * \fn Store
            * \brief Store a value in memory
            * \param address Memory address of the value to fetch
            * \param value The value to store
            * \return Error code
            */
            UInt8 Store(const UInt16 address, const UInt16 value);

        private:	// Stack helpers
            /**
            * \fn Pop
            * \brief Pop a value from the stack of the emulator and decrement the SP by 2
            * \param[out] value The value that was at the top of the stack
            * \return Error code
            */
            UInt8 Pop(UInt16 & value);

            /**
            * \fn Push
            * \brief Push a value on the stack and increment the SP by 2
            * \param val The value to be pushed
            * \return Error code
            */
            UInt8 Push(UInt16 val);

            /**
            * \fn PushPC
            * \brief Push the PC on the stack and increment the SP by 2
            * \return Error code
            */
            UInt8 PushPC();

        private:	// Flag register helpers
            /**
            * \fn SetSignZeroFlag
            * \brief Set the sign flag if the bit[15] of the result is lit
            *        and set the zero flag if the result is zero
            * \param result Result of an instruction
            */
            void SetSignZeroFlag(UInt16 result);

            /**
            * \fn SetCarryOverflowFlag*
            * \brief Set the carry and the overflow flag depending on the instruction used.
            * \param op1 The left hand side operand in a computation
            * \param op2 The right hand side operand in a computation
            */
            void SetCarryOverflowFlagAdd(UInt16 op1, UInt16 op2);
            void SetCarryOverflowFlagDiv(UInt16 op1, UInt16 op2);
            void SetCarryOverflowFlagMul(UInt16 op1, UInt16 op2);
            void SetCarryOverflowFlagSub(UInt16 op1, UInt16 op2);
        };
    }
}

#endif // CPU_H
