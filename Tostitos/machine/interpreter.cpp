#include "interpreter.h"

#include "constants.h"

#include <fstream>
#include <iostream>

using namespace MachineEngine::ProcessorSpace;

Interpreter::Interpreter() : mDist{ 0, std::numeric_limits<UInt16>::max() }, mErrorCode(NO_ERROR)
{
    mOps[0x00] = &Interpreter::NOP; 
    mOps[0x07] = &Interpreter::RND;

    mOps[0x10] = &Interpreter::DirectJMP;
    mOps[0x11] = &Interpreter::JMC;
    mOps[0x12] = &Interpreter::Jx;
    mOps[0x13] = &Interpreter::JME; 
    mOps[0x14] = &Interpreter::DirectCALL; 
    mOps[0x15] = &Interpreter::RET; 
    mOps[0x16] = &Interpreter::IndirectJMP; 
    mOps[0x17] = &Interpreter::Cx; 
    mOps[0x18] = &Interpreter::IndirectCALL;

    mOps[0x20] = &Interpreter::RegisterLDI;
    mOps[0x21] = &Interpreter::StackLDI;
    mOps[0x22] = &Interpreter::DirectLDM;
    mOps[0x23] = &Interpreter::IndirectLDM; 
    mOps[0x24] = &Interpreter::MOV;

    mOps[0x30] = &Interpreter::DirectSTM; 
    mOps[0x31] = &Interpreter::IndirectSTM;

    mOps[0x40] = &Interpreter::ADDI;
    mOps[0x41] = &Interpreter::InplaceADD;
    mOps[0x42] = &Interpreter::ADD;

    mOps[0x50] = &Interpreter::SUBI;
    mOps[0x51] = &Interpreter::InplaceSUB;
    mOps[0x52] = &Interpreter::SUB;

    mOps[0x53] = &Interpreter::CMPI;
    mOps[0x54] = &Interpreter::CMP;

    mOps[0x60] = &Interpreter::ANDI;
    mOps[0x61] = &Interpreter::InplaceAND;
    mOps[0x62] = &Interpreter::AND;
    
    mOps[0x63] = &Interpreter::TSTI;
    mOps[0x64] = &Interpreter::TST;

    mOps[0x70] = &Interpreter::ORI;
    mOps[0x71] = &Interpreter::InplaceOR;
    mOps[0x72] = &Interpreter::OR;

    mOps[0x80] = &Interpreter::XORI;
    mOps[0x81] = &Interpreter::InplaceXOR;
    mOps[0x82] = &Interpreter::XOR;

    mOps[0x90] = &Interpreter::MULI;
    mOps[0x91] = &Interpreter::InplaceMUL;
    mOps[0x92] = &Interpreter::MUL;
    
    mOps[0xA0] = &Interpreter::DIVI;
    mOps[0xA1] = &Interpreter::InplaceDIV;
    mOps[0xA2] = &Interpreter::DIV;
    mOps[0xA3] = &Interpreter::MODI;
    mOps[0xA4] = &Interpreter::InplaceMOD;
    mOps[0xA5] = &Interpreter::MOD;
    
    mOps[0xB0] = &Interpreter::NSHL;
    mOps[0xB1] = &Interpreter::NSHR;
    mOps[0xB2] = &Interpreter::NSAR;
    mOps[0xB3] = &Interpreter::RegisterSHL;
    mOps[0xB4] = &Interpreter::RegisterSHR;
    mOps[0xB5] = &Interpreter::RegisterSAR;

    mOps[0xC0] = &Interpreter::PUSH;
    mOps[0xC1] = &Interpreter::POP;
    mOps[0xC2] = &Interpreter::PUSHALL;
    mOps[0xC3] = &Interpreter::POPALL;
    mOps[0xC4] = &Interpreter::PUSHF;
    mOps[0xC5] = &Interpreter::POPF;

    mOps[0xE0] = &Interpreter::NOTI;
    mOps[0xE1] = &Interpreter::InplaceNOT;
    mOps[0xE2] = &Interpreter::NOT;
    mOps[0xE3] = &Interpreter::NEGI;
    mOps[0xE4] = &Interpreter::InplaceNEG;
    mOps[0xE5] = &Interpreter::NEG;
}

unsigned Interpreter::InterpretOne()
{
    Instruction inst = mCPU.FetchInstruction();
    InstructionExec func = mOps[inst.GetOpcode()];
    if(func)
        (this->*func)(inst);
    else
        std::cout << "Unknown opcode" << std::endl;
    
    return mErrorCode;
}

unsigned Interpreter::AcquireROM(const std::string & romName)
{
    std::fstream fileStream(romName, std::ios::in | std::ios::binary);

    if (fileStream.is_open())
    {
        fileStream.seekg(0, std::ios::end);
        auto size = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        std::vector<UInt8> romData(size);
        fileStream.read((char*)&romData[0], size);

        fileStream.close();

        // TODO: Should check for a valid header

        mCPU.InitPC(romData[0x0A]);

        romData.erase(romData.begin(), romData.begin() + HEADER_SIZE);

        mCPU.InitMemory(std::move(romData));

        return NO_ERROR;
    }
    else
    {
        return FILE_ERROR;
    }
}

void Interpreter::AcquireProgram(std::vector<UInt8> && program)
{
    mCPU.InitMemory(std::move(program));
}

const CPU & Interpreter::DumpCPUState() const
{
    return mCPU;
}

void Interpreter::Reset()
{
    mCPU.Reset();
}

/////////////// Arithmetic ///////////////

void Interpreter::ADDI(const Instruction & instruction)
{
    ImmediateBinaryArithmetic(instruction, std::plus<UInt16>(),
        [this](UInt16 op1, UInt16 op2) { mCPU.SetCarryOverflowFlagAdd(op1, op2); });
}

void Interpreter::InplaceADD(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::plus<UInt16>(),
        [this](UInt16 op1, UInt16 op2) { mCPU.SetCarryOverflowFlagAdd(op1, op2); }); 
}

void Interpreter::ADD(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::plus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagAdd(op1, op2); });
}

void Interpreter::SUBI(const Instruction & instruction)
{
    ImmediateBinaryArithmetic(instruction, std::minus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagSub(op1, op2); });
}

void Interpreter::InplaceSUB(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::minus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagSub(op1, op2); });
}

void Interpreter::SUB(const Instruction & instruction)
{
    BasicBinaryArithmetic(instruction, std::minus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagSub(op1, op2); });
}

void Interpreter::CMPI(const Instruction & instruction)
{ 
    DiscardImmediateBinaryArithmetic(instruction, std::minus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagSub(op1, op2); });
}

void Interpreter::CMP(const Instruction & instruction)
{ 
    DiscardBinaryArithmetic(instruction, std::minus<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagSub(op1, op2); });
}

void Interpreter::ANDI(const Instruction & instruction)
{ 
    ImmediateBinaryArithmetic(instruction, std::bit_and<UInt16>());
}

void Interpreter::InplaceAND(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::bit_and<UInt16>());
}

void Interpreter::AND(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::bit_and<UInt16>());
}

void Interpreter::TSTI(const Instruction & instruction)
{ 
    DiscardImmediateBinaryArithmetic(instruction, std::bit_and<UInt16>());
}

void Interpreter::TST(const Instruction & instruction)
{ 
    DiscardBinaryArithmetic(instruction, std::bit_and<UInt16>());
}

void Interpreter::ORI(const Instruction & instruction)
{ 
    ImmediateBinaryArithmetic(instruction, std::bit_or<UInt16>());
}

void Interpreter::InplaceOR(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::bit_or<UInt16>());
}

void Interpreter::OR(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::bit_or<UInt16>());
}

void Interpreter::XORI(const Instruction & instruction)
{ 
    ImmediateBinaryArithmetic(instruction, std::bit_xor<UInt16>());
}

void Interpreter::InplaceXOR(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::bit_xor<UInt16>());
}

void Interpreter::XOR(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::bit_xor<UInt16>());
}

void Interpreter::MULI(const Instruction & instruction)
{ 
    ImmediateBinaryArithmetic(instruction, std::multiplies<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagMul(op1, op2); });
}

void Interpreter::InplaceMUL(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::multiplies<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagMul(op1, op2); });
}

void Interpreter::MUL(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::multiplies<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagMul(op1, op2); });
}

void Interpreter::DIVI(const Instruction & instruction)
{ 
    ImmediateBinaryArithmetic(instruction, std::divides<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagDiv(op1, op2); });
}

void Interpreter::InplaceDIV(const Instruction & instruction)
{ 
    InplaceBinaryArithmetic(instruction, std::divides<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagDiv(op1, op2); });
}

void Interpreter::DIV(const Instruction & instruction)
{ 
    BasicBinaryArithmetic(instruction, std::divides<UInt16>(),
        [this](UInt16 op1, UInt16 op2){ mCPU.SetCarryOverflowFlagDiv(op1, op2); });
}

void Interpreter::MODI(const Instruction & instruction)
{
    ImmediateBinaryArithmetic(instruction, std::modulus<UInt16>());
}

void Interpreter::InplaceMOD(const Instruction & instruction)
{
    InplaceBinaryArithmetic(instruction, std::modulus<UInt16>());
}

void Interpreter::MOD(const Instruction & instruction)
{
    BasicBinaryArithmetic(instruction, std::modulus<UInt16>());
}

void Interpreter::NOTI(const Instruction & instruction)
{
    ImmediateUnaryArithmetic(instruction, std::bit_not<UInt16>());
}

void Interpreter::InplaceNOT(const Instruction & instruction)
{
    InplaceUnaryArithmetic(instruction, std::bit_not<UInt16>());
}

void Interpreter::NOT(const Instruction & instruction)
{
    BasicUnaryArithmetic(instruction, std::bit_not<UInt16>());
}

void Interpreter::NEGI(const Instruction & instruction)
{
    ImmediateUnaryArithmetic(instruction, std::negate<Int16>());
}

void Interpreter::InplaceNEG(const Instruction & instruction)
{
    InplaceUnaryArithmetic(instruction, std::negate<Int16>());
}

void Interpreter::NEG(const Instruction & instruction)
{
    BasicUnaryArithmetic(instruction, std::negate<Int16>());
}

/////////////// Call/Jump ///////////////

void Interpreter::DirectJMP(const Instruction & instruction)
{
    mErrorCode = mCPU.SetProgramCounter(instruction.GetImmediateValue());
}

void Interpreter::JMC(const Instruction & instruction)
{
    if (mCPU.DumpFlagRegister() & CPU::UNSIGNED_CARRY_FLAG)
        mErrorCode = mCPU.SetProgramCounter(instruction.GetImmediateValue());
}

void Interpreter::Jx(const Instruction & instruction)
{
    UInt8 condCode = instruction.GetFirstOperand();
    if(InterpretConditions(condCode))
        mCPU.SetProgramCounter(instruction.GetImmediateValue());
}

void Interpreter::JME(const Instruction & instruction)
{
    UInt16 xVal = mCPU.DumpRegister(instruction.GetFirstOperand());
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand());
    if(xVal == yVal)
        mErrorCode = mCPU.SetProgramCounter(instruction.GetImmediateValue());
}

void Interpreter::DirectCALL(const Instruction & instruction)
{
    mErrorCode = mCPU.PushPC();
    mErrorCode |= mCPU.SetProgramCounter(instruction.GetImmediateValue());
}

void Interpreter::RET(const Instruction & instruction)
{
    UInt16 val;
    mErrorCode = mCPU.Pop(val);
    mErrorCode |= mCPU.SetProgramCounter(val);
}

void Interpreter::IndirectJMP(const Instruction & instruction)
{
    mErrorCode = mCPU.SetProgramCounter(mCPU.DumpRegister(instruction.GetFirstOperand()));
}

void Interpreter::Cx(const Instruction & instruction)
{
    UInt8 condCode = instruction.GetFirstOperand();
    if(InterpretConditions(condCode))
    {
        mErrorCode = mCPU.PushPC();
        mErrorCode |= mCPU.SetProgramCounter(instruction.GetImmediateValue());
    }
}

void Interpreter::IndirectCALL(const Instruction & instruction)
{
    mErrorCode = mCPU.PushPC();
    mErrorCode |= mCPU.SetProgramCounter(mCPU.DumpRegister(instruction.GetFirstOperand()));
}

unsigned Interpreter::InterpretConditions(UInt8 condCode)
{
    UInt16 fr = mCPU.DumpFlagRegister();
    switch (condCode & 0xF)
    {
        case 0x0:	// Z
            return fr & CPU::ZERO_FLAG;
        case 0x1:	// NZ
            return fr ^ (fr | CPU::ZERO_FLAG);
        case 0x2:	// N
            return fr & CPU::NEGATIVE_FLAG;
        case 0x3:	// NN
            return (fr ^ (fr | CPU::NEGATIVE_FLAG));
        case 0x4:	// P
            return !(fr & CPU::ZERO_FLAG) && (fr ^ (fr | CPU::NEGATIVE_FLAG));
        case 0x5:	// O
            return fr & CPU::SIGNED_OVERFLOW_FLAG;
        case 0x6:	// NO
            return fr ^ (fr | CPU::SIGNED_OVERFLOW_FLAG);
        case 0x7:	// A
            return (fr ^ (fr | CPU::ZERO_FLAG)) && (fr ^ (fr | CPU::SIGNED_OVERFLOW_FLAG));
        case 0x8:	// AE
            return fr ^ (fr | CPU::UNSIGNED_CARRY_FLAG);
        case 0x9:	// B
            return fr & CPU::UNSIGNED_CARRY_FLAG;
        case 0xA:	// BE
            return (fr & CPU::ZERO_FLAG) || (fr & CPU::UNSIGNED_CARRY_FLAG);
        case 0xB:	// G
            return (fr & CPU::ZERO_FLAG) && ((fr & CPU::SIGNED_OVERFLOW_FLAG) == (fr & CPU::NEGATIVE_FLAG));
        case 0xC:	// GE
            return ((fr & CPU::SIGNED_OVERFLOW_FLAG) == (fr & CPU::NEGATIVE_FLAG));
        case 0xD:	// L
            return ((fr & CPU::SIGNED_OVERFLOW_FLAG) != (fr & CPU::NEGATIVE_FLAG));
        case 0xE:	// LE
            return fr & CPU::ZERO_FLAG || ((fr & CPU::SIGNED_OVERFLOW_FLAG) != (fr & CPU::NEGATIVE_FLAG));
        default:
            return 0;
    }
}

/////////////// Loads ///////////////

void Interpreter::RegisterLDI(const Instruction & instruction)
{
    mErrorCode = mCPU.SetRegister(instruction.GetFirstOperand(), instruction.GetImmediateValue());
}

void Interpreter::StackLDI(const Instruction & instruction)
{
    mErrorCode = mCPU.SetStackPointer(instruction.GetImmediateValue());
}

void Interpreter::DirectLDM(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    UInt16 iVal = instruction.GetImmediateValue();
    UInt16 val;
    mErrorCode = mCPU.Load(iVal, val);
    mErrorCode |= mCPU.SetRegister(addr, val);
}

void Interpreter::IndirectLDM(const Instruction & instruction)
{
    UInt8 addrX = instruction.GetFirstOperand();
    UInt8 addrY = instruction.GetSecondOperand();
    UInt16 val;
    mErrorCode = mCPU.Load(mCPU.DumpRegister(addrY), val);
    mErrorCode = mCPU.SetRegister(addrX, val);
}

void Interpreter::MOV(const Instruction & instruction)
{
    UInt8 addrX = instruction.GetFirstOperand();
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand()); 
    mErrorCode = mCPU.SetRegister(addrX, yVal);
}

/////////////// Misc ///////////////

void Interpreter::NOP(const Instruction & instruction) {  }

void Interpreter::RND(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    UInt16 maxVal = instruction.GetImmediateValue();
    UInt16 randVal = mDist(mRandEngine);
    while(randVal > maxVal)
        randVal= mDist(mRandEngine);
    mErrorCode = mCPU.SetRegister(addr, randVal);
}

/////////////// Push/Pop ///////////////

void Interpreter::PUSH(const Instruction & instruction)
{
    mErrorCode = mCPU.Push(mCPU.DumpRegister(instruction.GetFirstOperand()));
}

void Interpreter::POP(const Instruction & instruction)
{
    UInt16 val;
    mErrorCode = mCPU.Pop(val);
    mErrorCode |= mCPU.SetRegister(instruction.GetFirstOperand(), val);
}

void Interpreter::PUSHALL(const Instruction & instruction)
{
    for(UInt8 i = 0; i < CPU::NB_REGISTERS; ++i)
        mErrorCode |= mCPU.Push(mCPU.DumpRegister(i));
}

void Interpreter::POPALL(const Instruction & instruction)
{
    UInt16 val;
    for(int i = 15; i > -1; --i)
    {
        mErrorCode |= mCPU.Pop(val);
        mErrorCode |= mCPU.SetRegister(i, val);
    }
}

void Interpreter::PUSHF(const Instruction & instruction)
{
    mErrorCode = mCPU.Push(mCPU.DumpFlagRegister());
}

void Interpreter::POPF(const Instruction & instruction)
{
    UInt16 val;
    mErrorCode = mCPU.Pop(val);
    mCPU.SetFlagRegister(val);
}

/////////////// Shift ///////////////

void Interpreter::NSHL(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, LeftShift()(mCPU.DumpRegister(addr), instruction.GetThirdOperand()));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

void Interpreter::NSHR(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, LogicalRightShift()(mCPU.DumpRegister(addr), instruction.GetThirdOperand()));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

void Interpreter::NSAR(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, ArithmeticRightShift()(mCPU.DumpRegister(addr), instruction.GetThirdOperand()));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

void Interpreter::RegisterSHL(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, LeftShift()(mCPU.DumpRegister(addr), mCPU.DumpRegister(instruction.GetSecondOperand())));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

void Interpreter::RegisterSHR(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, LogicalRightShift()(mCPU.DumpRegister(addr), mCPU.DumpRegister(instruction.GetSecondOperand())));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

void Interpreter::RegisterSAR(const Instruction & instruction)
{
    UInt8 addr = instruction.GetFirstOperand();
    mErrorCode = mCPU.SetRegister(addr, ArithmeticRightShift()(mCPU.DumpRegister(addr), mCPU.DumpRegister(instruction.GetSecondOperand())));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(addr));
}

/////////////// Store ///////////////

void Interpreter::DirectSTM(const Instruction & instruction)
{
    UInt8 regAddr = instruction.GetFirstOperand();
    UInt16 memAddr = instruction.GetImmediateValue();
    mErrorCode = mCPU.Store(memAddr, mCPU.DumpRegister(regAddr));
}

void Interpreter::IndirectSTM(const Instruction & instruction)
{
    UInt16 xVal = mCPU.DumpRegister(instruction.GetFirstOperand());
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand());
    mErrorCode = mCPU.Store(yVal, xVal);
}

/////////////// Artihmetic Helpers ///////////////

void Interpreter::BasicBinaryArithmetic(const Instruction & instruction,
                                  std::function<UInt16(UInt16,UInt16)> ins, 
                                  std::function<void(UInt16,UInt16)> frh)
{
    UInt16 xVal = mCPU.DumpRegister(instruction.GetFirstOperand());
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand());
    
    if (frh)
        frh(xVal, yVal);
    
    UInt8 zReg = instruction.GetThirdOperand();
    mErrorCode = mCPU.SetRegister(zReg, ins(xVal, yVal));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(zReg));
}

void Interpreter::DiscardBinaryArithmetic(const Instruction & instruction,
                                    std::function<UInt16(UInt16,UInt16)> ins, 
                                    std::function<void(UInt16,UInt16)> frh)
{
    UInt16 xVal = mCPU.DumpRegister(instruction.GetFirstOperand());
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand());
    
    if (frh)
        frh(xVal, yVal);

    UInt16 result = ins(xVal, yVal);
    mCPU.SetSignZeroFlag(result);
}

void Interpreter::DiscardImmediateBinaryArithmetic(const Instruction & instruction,
                                             std::function<UInt16(UInt16,UInt16)> ins, 
                                             std::function<void(UInt16,UInt16)> frh)
{
    UInt16 xVal = mCPU.DumpRegister(instruction.GetFirstOperand());
    UInt16 iVal = instruction.GetImmediateValue();

    if (frh)
        frh(xVal, iVal);

    UInt16 result = ins(xVal, iVal);
    mCPU.SetSignZeroFlag(result);
}

void Interpreter::ImmediateBinaryArithmetic(const Instruction & instruction,
                                      std::function<UInt16(UInt16,UInt16)> ins, 
                                      std::function<void(UInt16,UInt16)> frh)
{
    UInt8 reg = instruction.GetFirstOperand();
    UInt16 iVal = instruction.GetImmediateValue();

    if (frh)
        frh(mCPU.DumpRegister(reg), iVal);
    
    mErrorCode = mCPU.SetRegister(reg, ins(mCPU.DumpRegister(reg), iVal));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(reg));
}

void Interpreter::InplaceBinaryArithmetic(const Instruction & instruction,
                                    std::function<UInt16(UInt16,UInt16)> ins, 
                                    std::function<void(UInt16,UInt16)> frh)
{
    UInt8 xReg = instruction.GetFirstOperand();
    UInt16 xVal = mCPU.DumpRegister(xReg);
    UInt16 yVal = mCPU.DumpRegister(instruction.GetSecondOperand());
    
    if (frh)
        frh(xVal, yVal);

    mErrorCode = mCPU.SetRegister(xReg, ins(xVal, yVal));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(xReg));
}

void Interpreter::BasicUnaryArithmetic(const Instruction & instruction,
    std::function<UInt16(UInt16)> ins)
{
    UInt16 val = mCPU.DumpRegister(instruction.GetSecondOperand());

    mErrorCode = mCPU.SetRegister(instruction.GetFirstOperand(), ins(val));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(instruction.GetFirstOperand()));
}

void Interpreter::ImmediateUnaryArithmetic(const Instruction & instruction,
    std::function<UInt16(UInt16)> ins)
{
    UInt8 reg = instruction.GetFirstOperand();
    UInt16 iVal = instruction.GetImmediateValue();

    mErrorCode = mCPU.SetRegister(reg, ins(iVal));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(reg));
}

void Interpreter::InplaceUnaryArithmetic(const Instruction & instruction,
    std::function<UInt16(UInt16)> ins)
{
    UInt8 reg = instruction.GetFirstOperand();
    UInt16 val = mCPU.DumpRegister(reg);

    mErrorCode = mCPU.SetRegister(reg, ins(val));
    mCPU.SetSignZeroFlag(mCPU.DumpRegister(reg));
}