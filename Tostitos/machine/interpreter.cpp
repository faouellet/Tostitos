#include "interpreter.h"

#include "constants.h"

#include <fstream>
#include <iostream>

using namespace MachineEngine::ProcessorSpace;

Interpreter::Interpreter() : m_Dist(0, std::numeric_limits<UInt16>::max()), m_ErrorCode(0)
{
    m_Ops[0x00] = &Interpreter::NOP; 
    m_Ops[0x07] = &Interpreter::RND;

    m_Ops[0x10] = &Interpreter::DirectJMP;
    m_Ops[0x11] = &Interpreter::JMC;
    m_Ops[0x12] = &Interpreter::Jx;
    m_Ops[0x13] = &Interpreter::JME; 
    m_Ops[0x14] = &Interpreter::DirectCALL; 
    m_Ops[0x15] = &Interpreter::RET; 
    m_Ops[0x16] = &Interpreter::IndirectJMP; 
    m_Ops[0x17] = &Interpreter::Cx; 
    m_Ops[0x18] = &Interpreter::IndirectCALL;

    m_Ops[0x20] = &Interpreter::RegisterLDI;
    m_Ops[0x21] = &Interpreter::StackLDI;
    m_Ops[0x22] = &Interpreter::DirectLDM;
    m_Ops[0x23] = &Interpreter::IndirectLDM; 
    m_Ops[0x24] = &Interpreter::MOV;

    m_Ops[0x30] = &Interpreter::DirectSTM; 
    m_Ops[0x31] = &Interpreter::IndirectSTM;

    m_Ops[0x40] = &Interpreter::ADDI;
    m_Ops[0x41] = &Interpreter::InplaceADD;
    m_Ops[0x42] = &Interpreter::ADD;

    m_Ops[0x50] = &Interpreter::SUBI;
    m_Ops[0x51] = &Interpreter::InplaceSUB;
    m_Ops[0x52] = &Interpreter::SUB;

    m_Ops[0x53] = &Interpreter::CMPI;
    m_Ops[0x54] = &Interpreter::CMP;

    m_Ops[0x60] = &Interpreter::ANDI;
    m_Ops[0x61] = &Interpreter::InplaceAND;
    m_Ops[0x62] = &Interpreter::AND;
    
    m_Ops[0x63] = &Interpreter::TSTI;
    m_Ops[0x64] = &Interpreter::TST;

    m_Ops[0x70] = &Interpreter::ORI;
    m_Ops[0x71] = &Interpreter::InplaceOR;
    m_Ops[0x72] = &Interpreter::OR;

    m_Ops[0x80] = &Interpreter::XORI;
    m_Ops[0x81] = &Interpreter::InplaceXOR;
    m_Ops[0x82] = &Interpreter::XOR;

    m_Ops[0x90] = &Interpreter::MULI;
    m_Ops[0x91] = &Interpreter::InplaceMUL;
    m_Ops[0x92] = &Interpreter::MUL;
    
    m_Ops[0xA0] = &Interpreter::DIVI;
    m_Ops[0xA1] = &Interpreter::InplaceDIV;
    m_Ops[0xA2] = &Interpreter::DIV;
    m_Ops[0xA3] = &Interpreter::MODI;
    m_Ops[0xA4] = &Interpreter::InplaceMOD;
    m_Ops[0xA5] = &Interpreter::MOD;
    
    m_Ops[0xB0] = &Interpreter::NSHL;
    m_Ops[0xB1] = &Interpreter::NSHR;
    m_Ops[0xB2] = &Interpreter::NSAR;
    m_Ops[0xB3] = &Interpreter::RegisterSHL;
    m_Ops[0xB4] = &Interpreter::RegisterSHR;
    m_Ops[0xB5] = &Interpreter::RegisterSAR;

    m_Ops[0xC0] = &Interpreter::PUSH;
    m_Ops[0xC1] = &Interpreter::POP;
    m_Ops[0xC2] = &Interpreter::PUSHALL;
    m_Ops[0xC3] = &Interpreter::POPALL;
    m_Ops[0xC4] = &Interpreter::PUSHF;
    m_Ops[0xC5] = &Interpreter::POPF;

    m_Ops[0xE0] = &Interpreter::NOTI;
    m_Ops[0xE1] = &Interpreter::InplaceNOT;
    m_Ops[0xE2] = &Interpreter::NOT;
}

unsigned Interpreter::InterpretOne()
{
    Instruction l_Inst = m_CPU.FetchInstruction();
    InstructionExec l_Func = m_Ops[l_Inst.GetOpcode()];
    if(l_Func)
        (this->*l_Func)(l_Inst);
    else
        std::cout << "Unknown opcode" << std::endl;
    
    return m_ErrorCode;
}

unsigned Interpreter::AcquireROM(const std::string & in_ROMName)
{
    std::fstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);

    if (l_FileStream.is_open())
    {
        l_FileStream.seekg(0, std::ios::end);
        auto size = l_FileStream.tellg();
        l_FileStream.seekg(0, std::ios::beg);

        std::vector<UInt8> l_ROMData(size);
        l_FileStream.read((char*)&l_ROMData[0], size);

        l_FileStream.close();

        // TODO: Should check for a valid header

        m_CPU.InitPC(l_ROMData[0x0A]);

        l_ROMData.erase(l_ROMData.begin(), l_ROMData.begin() + HEADER_SIZE);

        m_CPU.InitMemory(std::move(l_ROMData));

        return NoError;
    }
    else
    {
        return FileError;
    }
}

void Interpreter::AcquireProgram(std::vector<UInt8> && in_Program)
{
    m_CPU.InitMemory(std::move(in_Program));
}

const CPU & Interpreter::DumpCPUState() const
{
    return m_CPU;
}

void Interpreter::Reset()
{
    m_CPU.Reset();
}

/////////////// Arithmetic ///////////////

void Interpreter::ADDI(const Instruction & in_Instruction)
{
    ImmediateBinaryArithmetic(in_Instruction, std::plus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2) { m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); });
}

void Interpreter::InplaceADD(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::plus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2) { m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); 
}

void Interpreter::ADD(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::plus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); });
}

void Interpreter::SUBI(const Instruction & in_Instruction)
{
    ImmediateBinaryArithmetic(in_Instruction, std::minus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::InplaceSUB(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::minus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::SUB(const Instruction & in_Instruction)
{
    BasicBinaryArithmetic(in_Instruction, std::minus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::CMPI(const Instruction & in_Instruction)
{ 
    DiscardImmediateBinaryArithmetic(in_Instruction, std::minus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::CMP(const Instruction & in_Instruction)
{ 
    DiscardBinaryArithmetic(in_Instruction, std::minus<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::ANDI(const Instruction & in_Instruction)
{ 
    ImmediateBinaryArithmetic(in_Instruction, std::bit_and<UInt16>());
}

void Interpreter::InplaceAND(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::bit_and<UInt16>());
}

void Interpreter::AND(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::bit_and<UInt16>());
}

void Interpreter::TSTI(const Instruction & in_Instruction)
{ 
    DiscardImmediateBinaryArithmetic(in_Instruction, std::bit_and<UInt16>());
}

void Interpreter::TST(const Instruction & in_Instruction)
{ 
    DiscardBinaryArithmetic(in_Instruction, std::bit_and<UInt16>());
}

void Interpreter::ORI(const Instruction & in_Instruction)
{ 
    ImmediateBinaryArithmetic(in_Instruction, std::bit_or<UInt16>());
}

void Interpreter::InplaceOR(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::bit_or<UInt16>());
}

void Interpreter::OR(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::bit_or<UInt16>());
}

void Interpreter::XORI(const Instruction & in_Instruction)
{ 
    ImmediateBinaryArithmetic(in_Instruction, std::bit_xor<UInt16>());
}

void Interpreter::InplaceXOR(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::bit_xor<UInt16>());
}

void Interpreter::XOR(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::bit_xor<UInt16>());
}

void Interpreter::MULI(const Instruction & in_Instruction)
{ 
    ImmediateBinaryArithmetic(in_Instruction, std::multiplies<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::InplaceMUL(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::multiplies<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::MUL(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::multiplies<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::DIVI(const Instruction & in_Instruction)
{ 
    ImmediateBinaryArithmetic(in_Instruction, std::divides<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

void Interpreter::InplaceDIV(const Instruction & in_Instruction)
{ 
    InplaceBinaryArithmetic(in_Instruction, std::divides<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

void Interpreter::DIV(const Instruction & in_Instruction)
{ 
    BasicBinaryArithmetic(in_Instruction, std::divides<UInt16>(),
        [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

void Interpreter::MODI(const Instruction & in_Instruction)
{
    ImmediateBinaryArithmetic(in_Instruction, std::modulus<UInt16>());
}

void Interpreter::InplaceMOD(const Instruction & in_Instruction)
{
    InplaceBinaryArithmetic(in_Instruction, std::modulus<UInt16>());
}

void Interpreter::MOD(const Instruction & in_Instruction)
{
    BasicBinaryArithmetic(in_Instruction, std::modulus<UInt16>());
}

void Interpreter::NOTI(const Instruction & in_Instruction)
{
    ImmediateUnaryArithmetic(in_Instruction, std::bit_not<UInt16>());
}

void Interpreter::InplaceNOT(const Instruction & in_Instruction)
{
    InplaceUnaryArithmetic(in_Instruction, std::bit_not<UInt16>());
}

void Interpreter::NOT(const Instruction & in_Instruction)
{
    BasicUnaryArithmetic(in_Instruction, std::bit_not<UInt16>());
}

/////////////// Call/Jump ///////////////

void Interpreter::DirectJMP(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
}

void Interpreter::JMC(const Instruction & in_Instruction)
{
    if (m_CPU.DumpFlagRegister() & CPU::UnsignedCarryFlag)
        m_ErrorCode = m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
}

void Interpreter::Jx(const Instruction & in_Instruction)
{
    UInt8 l_CondCode = in_Instruction.GetFirstOperand();
    if(InterpretConditions(l_CondCode))
        m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
}

void Interpreter::JME(const Instruction & in_Instruction)
{
    UInt16 l_XVal = m_CPU.DumpRegister(in_Instruction.GetFirstOperand());
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());
    if(l_XVal == l_YVal)
        m_ErrorCode = m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
}

void Interpreter::DirectCALL(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.PushPC();
    m_ErrorCode |= m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
}

void Interpreter::RET(const Instruction & in_Instruction)
{
    UInt16 l_Val;
    m_ErrorCode = m_CPU.Pop(l_Val);
    m_ErrorCode |= m_CPU.SetProgramCounter(l_Val);
}

void Interpreter::IndirectJMP(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.SetProgramCounter(m_CPU.DumpRegister(in_Instruction.GetFirstOperand()));
}

void Interpreter::Cx(const Instruction & in_Instruction)
{
    UInt8 l_CondCode = in_Instruction.GetFirstOperand();
    if(InterpretConditions(l_CondCode))
    {
        m_ErrorCode = m_CPU.PushPC();
        m_ErrorCode |= m_CPU.SetProgramCounter(in_Instruction.GetImmediateValue());
    }
}

void Interpreter::IndirectCALL(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.PushPC();
    m_ErrorCode |= m_CPU.SetProgramCounter(m_CPU.DumpRegister(in_Instruction.GetFirstOperand()));
}

unsigned Interpreter::InterpretConditions(UInt8 in_CondCode)
{
    UInt16 l_FR = m_CPU.DumpFlagRegister();
    switch (in_CondCode & 0xF)
    {
        case 0x0:	// Z
            return l_FR & CPU::ZeroFlag;
        case 0x1:	// NZ
            return l_FR ^ (l_FR | CPU::ZeroFlag);
        case 0x2:	// N
            return l_FR & CPU::NegativeFlag;
        case 0x3:	// NN
            return (l_FR ^ (l_FR | CPU::NegativeFlag));
        case 0x4:	// P
            return !(l_FR & CPU::ZeroFlag) && (l_FR ^ (l_FR | CPU::NegativeFlag));
        case 0x5:	// O
            return l_FR & CPU::SignedOverflowFlag;
        case 0x6:	// NO
            return l_FR ^ (l_FR | CPU::SignedOverflowFlag);
        case 0x7:	// A
            return (l_FR ^ (l_FR | CPU::ZeroFlag)) && (l_FR ^ (l_FR | CPU::SignedOverflowFlag));
        case 0x8:	// AE
            return l_FR ^ (l_FR | CPU::UnsignedCarryFlag);
        case 0x9:	// B
            return l_FR & CPU::UnsignedCarryFlag;
        case 0xA:	// BE
            return (l_FR & CPU::ZeroFlag) || (l_FR & CPU::UnsignedCarryFlag);
        case 0xB:	// G
            return (l_FR & CPU::ZeroFlag) && ((l_FR & CPU::SignedOverflowFlag) == (l_FR & CPU::NegativeFlag));
        case 0xC:	// GE
            return ((l_FR & CPU::SignedOverflowFlag) == (l_FR & CPU::NegativeFlag));
        case 0xD:	// L
            return ((l_FR & CPU::SignedOverflowFlag) != (l_FR & CPU::NegativeFlag));
        case 0xE:	// LE
            return l_FR & CPU::ZeroFlag || ((l_FR & CPU::SignedOverflowFlag) != (l_FR & CPU::NegativeFlag));
        default:
            return 0;
    }
}

/////////////// Loads ///////////////

void Interpreter::RegisterLDI(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.SetRegister(in_Instruction.GetFirstOperand(), in_Instruction.GetImmediateValue());
}

void Interpreter::StackLDI(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.SetStackPointer(in_Instruction.GetImmediateValue());
}

void Interpreter::DirectLDM(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    UInt16 l_IVal = in_Instruction.GetImmediateValue();
    UInt16 l_Val;
    m_ErrorCode = m_CPU.Load(l_IVal, l_Val);
    m_ErrorCode |= m_CPU.SetRegister(l_Addr, l_Val);
}

void Interpreter::IndirectLDM(const Instruction & in_Instruction)
{
    UInt8 l_AddrX = in_Instruction.GetFirstOperand();
    UInt8 l_AddrY = in_Instruction.GetSecondOperand();
    UInt16 l_Val;
    m_ErrorCode = m_CPU.Load(m_CPU.DumpRegister(l_AddrY), l_Val);
    m_ErrorCode = m_CPU.SetRegister(l_AddrX, l_Val);
}

void Interpreter::MOV(const Instruction & in_Instruction)
{
    UInt8 l_AddrX = in_Instruction.GetFirstOperand();
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand()); 
    m_ErrorCode = m_CPU.SetRegister(l_AddrX, l_YVal);
}

/////////////// Misc ///////////////

void Interpreter::NOP(const Instruction & in_Instruction) {  }

void Interpreter::RND(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    UInt16 l_MaxVal = in_Instruction.GetImmediateValue();
    UInt16 l_RandVal = m_Dist(m_RandEngine);
    while(l_RandVal > l_MaxVal)
        l_RandVal= m_Dist(m_RandEngine);
    m_ErrorCode = m_CPU.SetRegister(l_Addr, l_RandVal);
}

/////////////// Push/Pop ///////////////

void Interpreter::PUSH(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.Push(m_CPU.DumpRegister(in_Instruction.GetFirstOperand()));
}

void Interpreter::POP(const Instruction & in_Instruction)
{
    UInt16 l_Val;
    m_ErrorCode = m_CPU.Pop(l_Val);
    m_ErrorCode |= m_CPU.SetRegister(in_Instruction.GetFirstOperand(), l_Val);
}

void Interpreter::PUSHALL(const Instruction & in_Instruction)
{
    for(UInt8 i = 0; i < CPU::NB_REGISTERS; ++i)
        m_ErrorCode |= m_CPU.Push(m_CPU.DumpRegister(i));
}

void Interpreter::POPALL(const Instruction & in_Instruction)
{
    UInt16 l_Val;
    for(int i = 15; i > -1; --i)
    {
        m_ErrorCode |= m_CPU.Pop(l_Val);
        m_ErrorCode |= m_CPU.SetRegister(i, l_Val);
    }
}

void Interpreter::PUSHF(const Instruction & in_Instruction)
{
    m_ErrorCode = m_CPU.Push(m_CPU.DumpFlagRegister());
}

void Interpreter::POPF(const Instruction & in_Instruction)
{
    UInt16 l_Val;
    m_ErrorCode = m_CPU.Pop(l_Val);
    m_CPU.SetFlagRegister(l_Val);
}

/////////////// Shift ///////////////

void Interpreter::NSHL(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, LeftShift()(m_CPU.DumpRegister(l_Addr), in_Instruction.GetThirdOperand()));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

void Interpreter::NSHR(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, LogicalRightShift()(m_CPU.DumpRegister(l_Addr), in_Instruction.GetThirdOperand()));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

void Interpreter::NSAR(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, ArithmeticRightShift()(m_CPU.DumpRegister(l_Addr), in_Instruction.GetThirdOperand()));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

void Interpreter::RegisterSHL(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, LeftShift()(m_CPU.DumpRegister(l_Addr), m_CPU.DumpRegister(in_Instruction.GetSecondOperand())));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

void Interpreter::RegisterSHR(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, LogicalRightShift()(m_CPU.DumpRegister(l_Addr), m_CPU.DumpRegister(in_Instruction.GetSecondOperand())));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

void Interpreter::RegisterSAR(const Instruction & in_Instruction)
{
    UInt8 l_Addr = in_Instruction.GetFirstOperand();
    m_ErrorCode = m_CPU.SetRegister(l_Addr, ArithmeticRightShift()(m_CPU.DumpRegister(l_Addr), m_CPU.DumpRegister(in_Instruction.GetSecondOperand())));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Addr));
}

/////////////// Store ///////////////

void Interpreter::DirectSTM(const Instruction & in_Instruction)
{
    UInt8 l_RegAddr = in_Instruction.GetFirstOperand();
    UInt16 l_MemAddr = in_Instruction.GetImmediateValue();
    m_ErrorCode = m_CPU.Store(l_MemAddr, m_CPU.DumpRegister(l_RegAddr));
}

void Interpreter::IndirectSTM(const Instruction & in_Instruction)
{
    UInt16 l_XVal = m_CPU.DumpRegister(in_Instruction.GetFirstOperand());
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());
    m_ErrorCode = m_CPU.Store(l_YVal, l_XVal);
}

/////////////// Artihmetic Helpers ///////////////

void Interpreter::BasicBinaryArithmetic(const Instruction & in_Instruction,
                                  std::function<UInt16(UInt16,UInt16)> in_Ins, 
                                  std::function<void(UInt16,UInt16)> in_FRH)
{
    UInt16 l_XVal = m_CPU.DumpRegister(in_Instruction.GetFirstOperand());
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());
    
    if (in_FRH)
        in_FRH(l_XVal, l_YVal);
    
    UInt8 l_ZReg = in_Instruction.GetThirdOperand();
    m_ErrorCode = m_CPU.SetRegister(l_ZReg, in_Ins(l_XVal, l_YVal));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_ZReg));
}

void Interpreter::DiscardBinaryArithmetic(const Instruction & in_Instruction,
                                    std::function<UInt16(UInt16,UInt16)> in_Ins, 
                                    std::function<void(UInt16,UInt16)> in_FRH)
{
    UInt16 l_XVal = m_CPU.DumpRegister(in_Instruction.GetFirstOperand());
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());
    
    if (in_FRH)
        in_FRH(l_XVal, l_YVal);

    UInt16 l_Result = in_Ins(l_XVal, l_YVal);
    m_CPU.SetSignZeroFlag(l_Result);
}

void Interpreter::DiscardImmediateBinaryArithmetic(const Instruction & in_Instruction,
                                             std::function<UInt16(UInt16,UInt16)> in_Ins, 
                                             std::function<void(UInt16,UInt16)> in_FRH)
{
    UInt16 l_XVal = m_CPU.DumpRegister(in_Instruction.GetFirstOperand());
    UInt16 l_IVal = in_Instruction.GetImmediateValue();

    if (in_FRH)
        in_FRH(l_XVal, l_IVal);

    UInt16 l_Result = in_Ins(l_XVal, l_IVal);
    m_CPU.SetSignZeroFlag(l_Result);
}

void Interpreter::ImmediateBinaryArithmetic(const Instruction & in_Instruction,
                                      std::function<UInt16(UInt16,UInt16)> in_Ins, 
                                      std::function<void(UInt16,UInt16)> in_FRH)
{
    UInt8 l_Reg = in_Instruction.GetFirstOperand();
    UInt16 l_IVal = in_Instruction.GetImmediateValue();

    if (in_FRH)
        in_FRH(m_CPU.DumpRegister(l_Reg), l_IVal);
    
    m_ErrorCode = m_CPU.SetRegister(l_Reg, in_Ins(m_CPU.DumpRegister(l_Reg), l_IVal));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Reg));
}

void Interpreter::InplaceBinaryArithmetic(const Instruction & in_Instruction,
                                    std::function<UInt16(UInt16,UInt16)> in_Ins, 
                                    std::function<void(UInt16,UInt16)> in_FRH)
{
    UInt8 l_XReg = in_Instruction.GetFirstOperand();
    UInt16 l_XVal = m_CPU.DumpRegister(l_XReg);
    UInt16 l_YVal = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());
    
    if (in_FRH)
        in_FRH(l_XVal, l_YVal);

    m_ErrorCode = m_CPU.SetRegister(l_XReg, in_Ins(l_XVal, l_YVal));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_XReg));
}

void Interpreter::BasicUnaryArithmetic(const Instruction & in_Instruction,
    std::function<UInt16(UInt16)> in_Ins)
{
    UInt16 l_Val = m_CPU.DumpRegister(in_Instruction.GetSecondOperand());

    m_ErrorCode = m_CPU.SetRegister(in_Instruction.GetFirstOperand(), in_Ins(l_Val));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(in_Instruction.GetFirstOperand()));
}

void Interpreter::ImmediateUnaryArithmetic(const Instruction & in_Instruction,
    std::function<UInt16(UInt16)> in_Ins)
{
    UInt8 l_Reg = in_Instruction.GetFirstOperand();
    UInt16 l_IVal = in_Instruction.GetImmediateValue();

    m_ErrorCode = m_CPU.SetRegister(l_Reg, in_Ins(l_IVal));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Reg));
}

void Interpreter::InplaceUnaryArithmetic(const Instruction & in_Instruction,
    std::function<UInt16(UInt16)> in_Ins)
{
    UInt8 l_Reg = in_Instruction.GetFirstOperand();
    UInt16 l_Val = m_CPU.DumpRegister(l_Reg);

    m_ErrorCode = m_CPU.SetRegister(l_Reg, in_Ins(l_Val));
    m_CPU.SetSignZeroFlag(m_CPU.DumpRegister(l_Reg));
}