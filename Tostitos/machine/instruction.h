#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "utils.h"

using MachineEngine::ProcessorSpace::Utils::UInt8;
using MachineEngine::ProcessorSpace::Utils::UInt16;
using MachineEngine::ProcessorSpace::Utils::UInt32;

namespace MachineEngine
{
    namespace ProcessorSpace
    {
        /**
        * \class Instruction
        * \brief Abstraction over a Chip16 instruction
        */
        class Instruction
        {
        private:
            UInt32 mValue;		    /*!< 4 byte value of the instruction */

            UInt16 mImmediateValue;	/*!< Last 2 bytes interpreted as an immediate value */

            UInt8 mOp1;		        /*!< 1st operand contained in the instruction */
            UInt8 mOp2;	    	    /*!< 2nd operand contained in the instruction */
            UInt8 mOp3; 		    /*!< 3rd operand contained in the instruction */
            UInt8 mOpcode;		    /*!< Opcode of the instruction */
            UInt8 mType;		    /*!< Type of the instruction */

            bool mUseImm;		    /*!< Does the instruction use an immediate value? */
            bool mIsInplace;	    /*!< Is the instruction done inplace? */

        public:
            /**
            * \enum
            * \brief Leading byte for the different types of instruction
            */
            enum
            {
                MISC = 0x00,
                JUMP = 0x10,
                LOAD = 0x20,
                STORE = 0x30,
                ADD = 0x40,
                SUB = 0x50,
                AND = 0x60,
                OR = 0x70,
                XOR = 0x80,
                MUL = 0x90,
                DIV = 0xA0,
                SHIFT = 0xB0,
                STACK = 0xC0,
                NOT = 0xE0
            };

        public:
            /**
            * \fn Instruction
            * \brief Default constructor
            * \param value Value of the instruction. 0 by default.
            */
            Instruction(const UInt32 value = 0);

            /**
            * \fn Instruction
            * \brief Destructor
            */
            ~Instruction() = default;

        private:
            /**
            * \fn FetchHalfByte
            * \brief Fetch half of an instruction byte
            * \param pos 0-based position of the half-byte within the instruction starting from the right
            * \return A 4 bits value
            */
            UInt8 FetchHalfByte(const UInt8 pos) const;

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
    }
}

#endif // INSTRUCTION_H
