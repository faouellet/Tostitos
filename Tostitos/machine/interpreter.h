#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "cpu.h"

#include <functional>
#include <memory>
#include <random>
#include <unordered_map>

using MachineEngine::ProcessorSpace::Utils::Int16;

namespace MachineEngine
{
    namespace ProcessorSpace
    {
        /**
        * \class Interpreter
        * \brief Interpret Chip16 instructions
        */
        class Interpreter
        {
        private:
            /**
            * \struct LeftShift
            * \brief Functor wrapping the left shift operator
            */
            struct LeftShift
            {
                UInt16 operator()(const UInt16 & lhs, const UInt16 & rhs)
                {
                    return lhs << rhs;
                }
            };

            /**
            * \struct ArithmeticRightShift
            * \brief Functor wrapping the right shift operator when the value to shift doesn't
            *        have its sign bit on. Otherwise, it uses a mask to copy the sign bit.
            */
            struct ArithmeticRightShift
            {
                UInt16 operator()(const Int16 & lhs, const UInt16 & rhs) const
                {
                    return lhs >> rhs;
                }
            };

            /**
            * \struct LogicalRightShift
            * \brief Functor wrapping the right shift operator
            */
            struct LogicalRightShift
            {
                UInt16 operator()(const UInt16 & lhs, const UInt16 & rhs) const
                {
                    return lhs >> rhs;
                }
            };

        private:
            typedef void (Interpreter::*InstructionExec) (const Instruction & instruction);
    
        private:
            UInt8 mErrorCode;									/*!< Current error code */

            CPU mCPU;											/*!< The central processing unit */

            std::mt19937 mRandEngine;							/*!< Random number engine */
            std::uniform_int_distribution<UInt16> mDist;		/*!< Distribution of the random numbers */

            std::unordered_map<UInt8, InstructionExec> mOps;	/*!< Interpretations of the opcodes */

        public:
            /**
            * \fn Interpreter
            * \brief Default constructor
            * \param in_CPU Pointer to a central processing unit implementation
            */
            Interpreter();
    
            /**
            * \fn ~Interpreter
            * \brief Destructor
            */
            ~Interpreter() = default;

        private:
            /**
            * \fn InitOpcodesTable
            * \brief Setup the opcode interpretation table
            */
            void InitOpcodesTable();

        public:
            /**
            * \fn AcquireROM
            * \brief Read a ROM from disk to the CPU memory
            * \param romName  The absolute path to the ROM
            * \return Error code depending on the situation
            */
            unsigned AcquireROM(const std::string & romName);

            /**
            * \fn AcquireProgram
            * \brief Initialize the interpreter with a program
            * \param program  The program to run
            */
            void AcquireProgram(std::vector<UInt8> && program);
                
            /**
            * \fn DumpCPUState
            * \brief Dump the CPU state (flags, memory, registers)
            * \return The CPU state
            */
            const CPU & DumpCPUState() const;

            /**
            * \fn InterpretOne
            * \brief Read an opcode from the ROM and execute it
            * \return An error code
            */
            unsigned InterpretOne();

            /**
            * \fn Reset
            * \brief Restore the interpreter at its pre-initialized state
            */
            void Reset();

        private:	// Arithmetic helpers
            /**
            * \fn BasicBinaryArithmetic
            * \brief Apply an instruction to two registers and store the result in a third register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply 
            * \param frh Handler responsible for updating the flag register
            */
            void BasicBinaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16,UInt16)> ins, 
                std::function<void(UInt16, UInt16)> frh = std::function<void(UInt16, UInt16)>());

            /**
            * \fn DiscardBinaryArithmetic
            * \brief Apply an instruction to two registers and discard the result
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply 
            * \param frh Handler responsible for updating the flag register
            */
            void DiscardBinaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16,UInt16)> ins, 
                std::function<void(UInt16, UInt16)> frh = std::function<void(UInt16, UInt16)>());

            /**
            * \fn DiscardImmediateBinaryArithmetic
            * \brief Apply an instruction to a register and an immediate value and discard the result
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply 
            * \param frh Handler responsible for updating the flag register
            */
            void DiscardImmediateBinaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16, UInt16)> ins,
                std::function<void(UInt16, UInt16)> frh = std::function<void(UInt16, UInt16)>());

            /**
            * \fn ImmediateBinaryArithmetic
            * \brief Apply an instruction to a register and an immediate value and store the result in the first register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply 
            * \param frh Handler responsible for updating the flag register
            */
            void ImmediateBinaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16, UInt16)> ins,
                std::function<void(UInt16, UInt16)> frh = std::function<void(UInt16, UInt16)>());

            /**
            * \fn InplaceBinaryArithmetic
            * \brief Apply an instruction to two registers and store the result in the first register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply 
            * \param frh Handler responsible for updating the flag register
            */
            void InplaceBinaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16, UInt16)> ins,
                std::function<void(UInt16, UInt16)> frh = std::function<void(UInt16, UInt16)>());

            /**
            * \fn BasicUnaryArithmetic
            * \brief Apply an instruction to a register and store the result in another register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply
            * \param frh Handler responsible for updating the flag register
            */
            void BasicUnaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16)> ins);

            /**
            * \fn ImmediateUnaryArithmetic
            * \brief Apply an instruction to an immediate value and store the result in a register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply
            * \param frh Handler responsible for updating the flag register
            */
            void ImmediateUnaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16)> ins);

            /**
            * \fn InplaceUnaryArithmetic
            * \brief Apply an instruction to one register and store the result in the register
            * \param instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param ins The instruction to apply
            * \param frh Handler responsible for updating the flag register
            */
            void InplaceUnaryArithmetic(const Instruction & instruction, std::function<UInt16(UInt16)> ins);
        private:
             /**
            * \fn InterpretConditions
            * \brief Decode and execute a condition
            * \param in_CondCode A byte containing the condition to be evaluated
            * \return Condition evalution result
            */
            unsigned InterpretConditions(UInt8 in_CondCode);

        private:	// Opcodes : See spec for more information
            void ADDI(const Instruction & instruction);
            void InplaceADD(const Instruction & instruction);
            void ADD(const Instruction & instruction);

            void SUBI(const Instruction & instruction);
            void InplaceSUB(const Instruction & instruction);
            void SUB(const Instruction & instruction);
            void CMPI(const Instruction & instruction);
            void CMP(const Instruction & instruction);
            
            void ANDI(const Instruction & instruction);
            void InplaceAND(const Instruction & instruction);
            void AND(const Instruction & instruction);
            void TSTI(const Instruction & instruction);
            void TST(const Instruction & instruction);
            
            void ORI(const Instruction & instruction);
            void InplaceOR(const Instruction & instruction);
            void OR(const Instruction & instruction);
            
            void XORI(const Instruction & instruction);
            void InplaceXOR(const Instruction & instruction);
            void XOR(const Instruction & instruction);
            
            void MULI(const Instruction & instruction);
            void InplaceMUL(const Instruction & instruction);
            void MUL(const Instruction & instruction);
            
            void DIVI(const Instruction & instruction);
            void InplaceDIV(const Instruction & instruction);
            void DIV(const Instruction & instruction);
            void MODI(const Instruction & instruction);
            void InplaceMOD(const Instruction & instruction);
            void MOD(const Instruction & instruction);

            void DirectJMP(const Instruction & instruction);
            void JMC(const Instruction & instruction);
            void Jx(const Instruction & instruction);
            void JME(const Instruction & instruction);
            void DirectCALL(const Instruction & instruction);
            void RET(const Instruction & instruction);
            void IndirectJMP(const Instruction & instruction);
            void Cx(const Instruction & instruction);
            void IndirectCALL(const Instruction & instruction);

            void RegisterLDI(const Instruction & instruction);
            void StackLDI(const Instruction & instruction);
            void DirectLDM(const Instruction & instruction);
            void IndirectLDM(const Instruction & instruction);
            void MOV(const Instruction & instruction);

            void NOP(const Instruction & instruction);
            void RND(const Instruction & instruction);

            void PUSH(const Instruction & instruction);
            void POP(const Instruction & instruction);
            void PUSHALL(const Instruction & instruction);
            void POPALL(const Instruction & instruction);
            void PUSHF(const Instruction & instruction);
            void POPF(const Instruction & instruction);

            void NSHL(const Instruction & instruction);
            void NSHR(const Instruction & instruction);
            void NSAR(const Instruction & instruction);
            void RegisterSHL(const Instruction & instruction);
            void RegisterSHR(const Instruction & instruction);
            void RegisterSAR(const Instruction & instruction);

            void DirectSTM(const Instruction & instruction);
            void IndirectSTM(const Instruction & instruction);

            void NOTI(const Instruction & instruction);
            void InplaceNOT(const Instruction & instruction);
            void NOT(const Instruction & instruction);

            void NEGI(const Instruction & instruction);
            void InplaceNEG(const Instruction & instruction);
            void NEG(const Instruction & instruction);
        };
    }
}

#endif // INTERPRETER_H
