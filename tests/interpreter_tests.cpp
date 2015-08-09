#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Interpreter
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "interpreter_tests.h"

#include "constants.h"

BOOST_FIXTURE_TEST_SUITE( InterpreterTestSuite, InterpreterFixture )

BOOST_AUTO_TEST_CASE( InitMemoryTest )
{
    // BOOST_REQUIRE_EQUAL(Interpret.AcquireProgram(std::move(AddTestData)), NoError);
    Interpret.AcquireProgram(std::move(AddTestData));
    std::vector<Utils::UInt8> l_MemoryDump(Interpret.DumpCPUState().DumpMemory());

    for(unsigned i = 0; i < AddTestData.size(); ++i)
        BOOST_REQUIRE_EQUAL(AddTestData[i], l_MemoryDump[i]);

    // BOOST_REQUIRE_EQUAL(Interpret.AcquireProgram(std::vector<UInt8>()), EmptyROMError);
    // BOOST_REQUIRE_EQUAL(Interpret.AcquireProgram(std::vector<UInt8>(2147483647, 42)), ROMOverflowError);
}

BOOST_AUTO_TEST_CASE( AddTest )
{
    Interpret.AcquireProgram(std::move(AddTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();										// ADDI : R0 += 0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    Interpret.InterpretOne();										// ADDI : R0 += 65535
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// ADD : R1 += R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// ADD : R2 = R0 + R3
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// ADD : R3 = R0 + R1	(overflow)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x40);					// Overflow flag set
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
    std::vector<UInt16> l_AddDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_AddDump[0], 65535);
    BOOST_REQUIRE_EQUAL(l_AddDump[1], 65535);
    BOOST_REQUIRE_EQUAL(l_AddDump[2], 65535);
    BOOST_REQUIRE_EQUAL(l_AddDump[3], 65534);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_AddDump[i], 0);
}

BOOST_AUTO_TEST_CASE( AndTest )
{
    Interpret.AcquireProgram(std::move(AndTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();						// ANDI : R0 & 65535
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
    Interpret.InterpretOne();						// AND : R1 &= R0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
    Interpret.InterpretOne();						// AND : R2 = R0 & R1
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
    //Cpu.InterpretOne();	// TSTI
    //Cpu.InterpretOne();	// TST
    std::vector<UInt16> l_AndDump(Cpu.DumpRegisters());

    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_AndDump[i], 0);
}

BOOST_AUTO_TEST_CASE( DivTest )
{
    Interpret.AcquireProgram(std::move(DivTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();	// ADDI : R0 += 8
    Interpret.InterpretOne();	// ADDI : R1 += 4
    Interpret.InterpretOne();	// ADDI : R2 += 2
    Interpret.InterpretOne();	// DIVI : R0 /= 32
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    Interpret.InterpretOne();										// DIV : R1 /= R2
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    Interpret.InterpretOne();										// DIV : R3 = R2 / R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    std::vector<UInt16> l_DivDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_DivDump[0], 0);
    BOOST_REQUIRE_EQUAL(l_DivDump[1], 2);
    BOOST_REQUIRE_EQUAL(l_DivDump[2], 2);
    BOOST_REQUIRE_EQUAL(l_DivDump[3], 1);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_DivDump[i], 0);
}

BOOST_AUTO_TEST_CASE( ModTest )
{
    Interpret.AcquireProgram(std::move(ModTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();										// ADDI : R0 += 8
    Interpret.InterpretOne();										// MODI : R0 = R0 % 32
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 8);

    Interpret.InterpretOne();						                // MODI : R0 = R0 % 32
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	                // Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 0);

    Interpret.InterpretOne();										// ADDI : R1 += 4
    Interpret.InterpretOne();										// ADDI : R2 += 5
    Interpret.InterpretOne();										// MOD : R2 = R2 % R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(2), 1);
    Interpret.InterpretOne();										// MOD : R1 = R1 % R1
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(1), 0);

    Interpret.InterpretOne();										// ADDI : R3 += 4
    Interpret.InterpretOne();										// ADDI : R4 += 5
    Interpret.InterpretOne();										// MOD : R5 = R4 % R3
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(5), 1);
    Interpret.InterpretOne();										// MOD : R5 = R3 % R2
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);				    // Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(5), 0);
}

BOOST_AUTO_TEST_CASE( ErrorTest )
{
    Interpret.AcquireProgram(std::move(ErrorTestData));
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::StackUnderflow);
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::MemoryError);
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::MemoryError);
}

BOOST_AUTO_TEST_CASE( MulTest )
{
    Interpret.AcquireProgram(std::move(MulTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();	// ADDI : R0 += 2
    Interpret.InterpretOne();	// ADDI : R1 += 2
    Interpret.InterpretOne();	// MULI : R0 *= 10
    Interpret.InterpretOne();	// MUL : R1 *= R0
    Interpret.InterpretOne();	// MUL : R2 = R1 * R0
    Interpret.InterpretOne();	// MUL : R3 *= 0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    Interpret.InterpretOne();										// MUL : R3 = R2 * R2 (overflow)
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
    std::vector<UInt16> l_MulDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_MulDump[0], 20);
    BOOST_REQUIRE_EQUAL(l_MulDump[1], 40);
    BOOST_REQUIRE_EQUAL(l_MulDump[2], 800);
    BOOST_REQUIRE_EQUAL(l_MulDump[3], 50176);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_MulDump[i], 0);
}

BOOST_AUTO_TEST_CASE( OrTest )
{
    Interpret.AcquireProgram(std::move(OrTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();										// ORI : R0 | 0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    Interpret.InterpretOne();										// ORI : R0 | 1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    Interpret.InterpretOne();										// OR : R1 |= R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    Interpret.InterpretOne();										// OR : R2 = R0 | R3
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    std::vector<UInt16> l_OrDump(Cpu.DumpRegisters());

    for(int i = 0; i < 3; ++i)
        BOOST_REQUIRE_EQUAL(l_OrDump[i], 1);

    for(int i = 3; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_OrDump[i], 0);
}

BOOST_AUTO_TEST_CASE( SubTest )
{
    Interpret.AcquireProgram(std::move(SubTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();	// ADDI : R0 += 12
    Interpret.InterpretOne();	// SUBI : R0 -= 4
    Interpret.InterpretOne();	// SUB : R1 -= 4
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
    Interpret.InterpretOne();										// SUB : R0 -= R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
    Interpret.InterpretOne();										// SUB : R1 -= R0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SUB : R2 = R0 - R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
    Interpret.InterpretOne();										// SUB : R3 = R1 - R0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    //Cpu.InterpretOne();	// CMPI
    //Cpu.InterpretOne();	// CMP
    std::vector<UInt16> l_SubDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_SubDump[0], 12);
    BOOST_REQUIRE_EQUAL(l_SubDump[1], 65520);
    BOOST_REQUIRE_EQUAL(l_SubDump[2], 28);
    BOOST_REQUIRE_EQUAL(l_SubDump[3], 65508);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_SubDump[i], 0);
}

BOOST_AUTO_TEST_CASE( XorTest )
{
    Interpret.AcquireProgram(std::move(XorTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();										// XORI : R0 ^ 1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    Interpret.InterpretOne();										// XOR : R1 ^= R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    Interpret.InterpretOne();										// XOR : R2 = R0 ^ R3
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    std::vector<UInt16> l_XorDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_XorDump[0], 1);
    BOOST_REQUIRE_EQUAL(l_XorDump[1], 1);

    for(int i = 2; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_XorDump[i], 0);
}

// TODO : Call/Jump tests 

BOOST_AUTO_TEST_CASE( MemoryTest )
{
    Interpret.AcquireProgram(std::move(MemoryTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();	// LDI : R0 = 255
    Interpret.InterpretOne();	// STM : Memory[R0] = R0
    Interpret.InterpretOne();	// LDI : SP = SP + 1
    Interpret.InterpretOne();	// STM : Memory[F0] = R0
    Interpret.InterpretOne();	// LDM : R2 = Memory[F0]
    Interpret.InterpretOne();	// LDM : R3 = Memory[R0]
    Interpret.InterpretOne();	// MOV : R2 = R0
    std::vector<UInt16> l_RegisterDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_RegisterDump[0], 255);
    BOOST_REQUIRE_EQUAL(l_RegisterDump[1], 0);
    BOOST_REQUIRE_EQUAL(l_RegisterDump[2], 255);
    BOOST_REQUIRE_EQUAL(l_RegisterDump[3], 255);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_RegisterDump[i], 0);

    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 1);

    std::vector<UInt8> l_MemoryDump(Cpu.DumpMemory());
    BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]], l_RegisterDump[0] & 0x00FF);
    BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]+1], l_RegisterDump[0] & 0xFF00);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
    Interpret.AcquireProgram(std::move(StackTestData));
    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// LDI : Ri = i * 2

    Interpret.InterpretOne();	// PUSHALL

    const CPU& Cpu = Interpret.DumpCPUState();
    std::vector<Utils::UInt8> l_MemoryDump(Cpu.DumpMemory());
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_MemoryDump[i * 2 + STACK_START], i * 2);
    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 32);

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// LDI : Ri = 0

    Interpret.InterpretOne();	// POPALL

    std::vector<Utils::UInt16> l_RegistersDump(Cpu.DumpRegisters());
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_RegistersDump[i], i * 2);
    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START);

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// PUSH Ri

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// POP Ri

    l_RegistersDump = Cpu.DumpRegisters();
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_RegistersDump[i], (NB_REGISTERS - 1 - i) * 2);
    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START);
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
    Interpret.AcquireProgram(std::move(ShiftTestData));
    for(int i = 0; i < NB_REGISTERS/2; ++i)
        Interpret.InterpretOne();	// ADDI : Ri += 65535

    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();									    // SHL : R0 << 13 (Logical)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SHR : R1 >> 13 (Logical)
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
    Interpret.InterpretOne();										// SAL : R2 << 13 (Arithmetic)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SAR : R3 >> 13 (Arithmetic)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SHL : R4 << R1 (Logical)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SHR : R5 >> R1 (Logical)
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
    Interpret.InterpretOne();										// SAL : R6 << R1 (Arithmetic)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
    Interpret.InterpretOne();										// SAR : R7 >> R1 (Arithmetic)
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set

    std::vector<UInt16> l_ShiftDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(l_ShiftDump[0], 57344);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[1], 7);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[2], 57344);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[3], 65535);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[4], 65408);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[5], 511);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[6], 65408);
    BOOST_REQUIRE_EQUAL(l_ShiftDump[7], 65535);
    
    for(int i = NB_REGISTERS/2; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_ShiftDump[i], 0);
}

BOOST_AUTO_TEST_CASE( RndTest )
{
    Interpret.AcquireProgram(std::move(RndTestData));
    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();

    std::vector<UInt16> l_RndDump(Interpret.DumpCPUState().DumpRegisters());
    for(int i = 0; i < NB_REGISTERS; ++i)
    {
        BOOST_REQUIRE_GE(l_RndDump[i], 0);
        BOOST_REQUIRE_LE(l_RndDump[i], 0xFF00);
    }
}

BOOST_AUTO_TEST_SUITE_END()
