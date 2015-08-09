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
                UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS)
                {
                    return in_LHS << in_RHS;
                }
            };

            /**
            * \struct ArithmeticRightShift
            * \brief Functor wrapping the right shift operator when the value to shift doesn't
            *        have its sign bit on. Otherwise, it uses a mask to copy the sign bit.
            */
            struct ArithmeticRightShift
            {
                UInt16 operator()(const Int16 & in_LHS, const UInt16 & in_RHS) const
                {
                    return in_LHS >> in_RHS;
                }
            };

            /**
            * \struct LogicalRightShift
            * \brief Functor wrapping the right shift operator
            */
            struct LogicalRightShift
            {
                UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS) const
                {
                    return in_LHS >> in_RHS;
                }
            };

        private:
            typedef void (Interpreter::*InstructionExec) (const Instruction & in_Instruction);
    
        private:
            UInt8 m_ErrorCode;									/*!< Current error code */

            CPU m_CPU;											/*!< The central processing unit */

            std::mt19937 m_RandEngine;							/*!< Random number engine */
            std::uniform_int_distribution<UInt16> m_Dist;		/*!< Distribution of the random numbers */

            std::unordered_map<UInt8, InstructionExec> m_Ops;	/*!< Interpretations of the opcodes */

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
            * \param in_ROMName  The absolute path to the ROM
            * \return Error code depending on the situation
            */
            unsigned AcquireROM(const std::string & in_ROMName);

            /**
            * \fn AcquireProgram
            * \brief Initialize the interpreter with a program
            * \param in_Program  The program to run
            */
            void AcquireProgram(std::vector<UInt8> && in_Program);
                
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
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply 
            * \param in_FRH Handler responsible for updating the flag register
            */
            void BasicBinaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
                std::function<void(UInt16, UInt16)> in_FRH = std::function<void(UInt16, UInt16)>());

            /**
            * \fn DiscardBinaryArithmetic
            * \brief Apply an instruction to two registers and discard the result
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply 
            * \param in_FRH Handler responsible for updating the flag register
            */
            void DiscardBinaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
                std::function<void(UInt16, UInt16)> in_FRH = std::function<void(UInt16, UInt16)>());

            /**
            * \fn DiscardImmediateBinaryArithmetic
            * \brief Apply an instruction to a register and an immediate value and discard the result
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply 
            * \param in_FRH Handler responsible for updating the flag register
            */
            void DiscardImmediateBinaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16, UInt16)> in_Ins,
                std::function<void(UInt16, UInt16)> in_FRH = std::function<void(UInt16, UInt16)>());

            /**
            * \fn ImmediateBinaryArithmetic
            * \brief Apply an instruction to a register and an immediate value and store the result in the first register
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply 
            * \param in_FRH Handler responsible for updating the flag register
            */
            void ImmediateBinaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16, UInt16)> in_Ins,
                std::function<void(UInt16, UInt16)> in_FRH = std::function<void(UInt16, UInt16)>());

            /**
            * \fn InplaceBinaryArithmetic
            * \brief Apply an instruction to two registers and store the result in the first register
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply 
            * \param in_FRH Handler responsible for updating the flag register
            */
            void InplaceBinaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16, UInt16)> in_Ins,
                std::function<void(UInt16, UInt16)> in_FRH = std::function<void(UInt16, UInt16)>());

            /**
            * \fn BasicUnaryArithmetic
            * \brief Apply an instruction to a register and store the result in another register
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply
            * \param in_FRH Handler responsible for updating the flag register
            */
            void BasicUnaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16)> in_Ins);

            /**
            * \fn ImmediateUnaryArithmetic
            * \brief Apply an instruction to an immediate value and store the result in a register
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply
            * \param in_FRH Handler responsible for updating the flag register
            */
            void ImmediateUnaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16)> in_Ins);

            /**
            * \fn InplaceUnaryArithmetic
            * \brief Apply an instruction to one register and store the result in the register
            * \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
            * \param in_Ins The instruction to apply
            * \param in_FRH Handler responsible for updating the flag register
            */
            void InplaceUnaryArithmetic(const Instruction & in_Instruction, std::function<UInt16(UInt16)> in_Ins);
        private:
             /**
            * \fn InterpretConditions
            * \brief Decode and execute a condition
            * \param in_CondCode A byte containing the condition to be evaluated
            * \return Condition evalution result
            */
            unsigned InterpretConditions(UInt8 in_CondCode);

        private:	// Opcodes : See spec for more information
            void ADDI(const Instruction & in_Instruction);
            void InplaceADD(const Instruction & in_Instruction);
            void ADD(const Instruction & in_Instruction);

            void SUBI(const Instruction & in_Instruction);
            void InplaceSUB(const Instruction & in_Instruction);
            void SUB(const Instruction & in_Instruction);
            void CMPI(const Instruction & in_Instruction);
            void CMP(const Instruction & in_Instruction);
            
            void ANDI(const Instruction & in_Instruction);
            void InplaceAND(const Instruction & in_Instruction);
            void AND(const Instruction & in_Instruction);
            void TSTI(const Instruction & in_Instruction);
            void TST(const Instruction & in_Instruction);
            
            void ORI(const Instruction & in_Instruction);
            void InplaceOR(const Instruction & in_Instruction);
            void OR(const Instruction & in_Instruction);
            
            void XORI(const Instruction & in_Instruction);
            void InplaceXOR(const Instruction & in_Instruction);
            void XOR(const Instruction & in_Instruction);
            
            void MULI(const Instruction & in_Instruction);
            void InplaceMUL(const Instruction & in_Instruction);
            void MUL(const Instruction & in_Instruction);
            
            void DIVI(const Instruction & in_Instruction);
            void InplaceDIV(const Instruction & in_Instruction);
            void DIV(const Instruction & in_Instruction);
            void MODI(const Instruction & in_Instruction);
            void InplaceMOD(const Instruction & in_Instruction);
            void MOD(const Instruction & in_Instruction);

            void DirectJMP(const Instruction & in_Instruction);
            void JMC(const Instruction & in_Instruction);
            void Jx(const Instruction & in_Instruction);
            void JME(const Instruction & in_Instruction);
            void DirectCALL(const Instruction & in_Instruction);
            void RET(const Instruction & in_Instruction);
            void IndirectJMP(const Instruction & in_Instruction);
            void Cx(const Instruction & in_Instruction);
            void IndirectCALL(const Instruction & in_Instruction);

            void RegisterLDI(const Instruction & in_Instruction);
            void StackLDI(const Instruction & in_Instruction);
            void DirectLDM(const Instruction & in_Instruction);
            void IndirectLDM(const Instruction & in_Instruction);
            void MOV(const Instruction & in_Instruction);

            void NOP(const Instruction & in_Instruction);
            void RND(const Instruction & in_Instruction);

            void PUSH(const Instruction & in_Instruction);
            void POP(const Instruction & in_Instruction);
            void PUSHALL(const Instruction & in_Instruction);
            void POPALL(const Instruction & in_Instruction);
            void PUSHF(const Instruction & in_Instruction);
            void POPF(const Instruction & in_Instruction);

            void NSHL(const Instruction & in_Instruction);
            void NSHR(const Instruction & in_Instruction);
            void NSAR(const Instruction & in_Instruction);
            void RegisterSHL(const Instruction & in_Instruction);
            void RegisterSHR(const Instruction & in_Instruction);
            void RegisterSAR(const Instruction & in_Instruction);

            void DirectSTM(const Instruction & in_Instruction);
            void IndirectSTM(const Instruction & in_Instruction);

            void NOTI(const Instruction & in_Instruction);
            void InplaceNOT(const Instruction & in_Instruction);
            void NOT(const Instruction & in_Instruction);
        };
    }
}

#endif // INTERPRETER_H
