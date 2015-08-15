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

#include <limits>

BOOST_FIXTURE_TEST_SUITE( InterpreterTestSuite, InterpreterFixture )

BOOST_AUTO_TEST_CASE( InitMemoryTest )
{
    // BOOST_REQUIRE_EQUAL(Interpret.AcquireProgram(std::move(AddTestData)), NoError);
    Interpret.AcquireProgram(std::move(AddTestData));
    std::vector<Utils::UInt8> memoryDump(Interpret.DumpCPUState().DumpMemory());

    for(unsigned i = 0; i < AddTestData.size(); ++i)
        BOOST_REQUIRE_EQUAL(AddTestData[i], memoryDump[i]);

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
    std::vector<UInt16> addDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(addDump[0], 65535);
    BOOST_REQUIRE_EQUAL(addDump[1], 65535);
    BOOST_REQUIRE_EQUAL(addDump[2], 65535);
    BOOST_REQUIRE_EQUAL(addDump[3], 65534);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(addDump[i], 0);
}

BOOST_AUTO_TEST_CASE( AndTest )
{
    Interpret.AcquireProgram(std::move(AndTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    Interpret.InterpretOne();						                // ANDI : R0 & 65535
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	                // Zero flag set
    Interpret.InterpretOne();						                // AND : R1 &= R0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	                // Zero flag set
    Interpret.InterpretOne();						                // AND : R2 = R0 & R1
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	                // Zero flag set
    
    Interpret.InterpretOne();	                                    // ADDI : R0 += 1
    Interpret.InterpretOne();	                                    // TSTI : R0 & 65535
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 1);
    Interpret.InterpretOne();	                                    // TST : R0 & R1
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	                // Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 1);
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(1), 0);

    std::vector<UInt16> andDump(Cpu.DumpRegisters());
    for(int i = 1; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(andDump[i], 0);
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
    std::vector<UInt16> divDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(divDump[0], 0);
    BOOST_REQUIRE_EQUAL(divDump[1], 2);
    BOOST_REQUIRE_EQUAL(divDump[2], 2);
    BOOST_REQUIRE_EQUAL(divDump[3], 1);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(divDump[i], 0);
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

    for (int i = 5; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(i), 0);
}

BOOST_AUTO_TEST_CASE( ErrorTest )
{
    Interpret.AcquireProgram(std::move(ErrorTestData));
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::STACK_UNDERFLOW);
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::MEMORY_ERROR);
    BOOST_REQUIRE_EQUAL(Interpret.InterpretOne(), CPU::MEMORY_ERROR);
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
    std::vector<UInt16> mulDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(mulDump[0], 20);
    BOOST_REQUIRE_EQUAL(mulDump[1], 40);
    BOOST_REQUIRE_EQUAL(mulDump[2], 800);
    BOOST_REQUIRE_EQUAL(mulDump[3], 50176);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(mulDump[i], 0);
}

// TODO: Add better support for signed operation
BOOST_AUTO_TEST_CASE( NegTest )
{
    Interpret.AcquireProgram(std::move(NegTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    const UInt16 l_MaxLimit = std::numeric_limits<UInt16>::max();
    Interpret.InterpretOne();                                       // NEGI : R0 = -1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit);

    Interpret.InterpretOne();                                       // NEG : R0 = -R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 1);
    Interpret.InterpretOne();                                       // NEG : R0 = -R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit);

    Interpret.InterpretOne();                                       // ADDI : R1 += 2
    Interpret.InterpretOne();                                       // NEG : R0 = -R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit - 1);
}

BOOST_AUTO_TEST_CASE( NotTest )
{
    Interpret.AcquireProgram(std::move(NotTestData));
    const CPU& Cpu = Interpret.DumpCPUState();
    const UInt16 l_MaxLimit = std::numeric_limits<UInt16>::max();
    Interpret.InterpretOne();                                       // NOTI : R0 = !65535
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 0);

    Interpret.InterpretOne();                                       // NOTI : R0 = !0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit);

    Interpret.InterpretOne();                                       // NOT : R0 = !R0
    BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), 0);

    Interpret.InterpretOne();                                       // NOT : R0 = !R0
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit);

    Interpret.InterpretOne();                                       // ADDI : R1 += 2

    Interpret.InterpretOne();                                       // NOT : R0 = !R1
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit - 2);

    Interpret.InterpretOne();                                       // NOT : R0 = !R2
    BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
    BOOST_REQUIRE_EQUAL(Cpu.DumpRegister(0), l_MaxLimit);

    std::vector<UInt16> l_NotDump(Cpu.DumpRegisters());
    for (int i = 2; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(l_NotDump[i], 0);
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
    std::vector<UInt16> orDump(Cpu.DumpRegisters());

    for(int i = 0; i < 3; ++i)
        BOOST_REQUIRE_EQUAL(orDump[i], 1);

    for(int i = 3; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(orDump[i], 0);
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
    std::vector<UInt16> subDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(subDump[0], 12);
    BOOST_REQUIRE_EQUAL(subDump[1], 65520);
    BOOST_REQUIRE_EQUAL(subDump[2], 28);
    BOOST_REQUIRE_EQUAL(subDump[3], 65508);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(subDump[i], 0);
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
    std::vector<UInt16> xorDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(xorDump[0], 1);
    BOOST_REQUIRE_EQUAL(xorDump[1], 1);

    for(int i = 2; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(xorDump[i], 0);
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
    std::vector<UInt16> registerDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(registerDump[0], 255);
    BOOST_REQUIRE_EQUAL(registerDump[1], 0);
    BOOST_REQUIRE_EQUAL(registerDump[2], 255);
    BOOST_REQUIRE_EQUAL(registerDump[3], 255);

    for(int i = 4; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(registerDump[i], 0);

    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 1);

    std::vector<UInt8> memoryDump(Cpu.DumpMemory());
    BOOST_REQUIRE_EQUAL(memoryDump[registerDump[0]], registerDump[0] & 0x00FF);
    BOOST_REQUIRE_EQUAL(memoryDump[registerDump[0]+1], registerDump[0] & 0xFF00);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
    Interpret.AcquireProgram(std::move(StackTestData));
    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// LDI : Ri = i * 2

    Interpret.InterpretOne();	// PUSHALL

    const CPU& Cpu = Interpret.DumpCPUState();
    std::vector<Utils::UInt8> memoryDump(Cpu.DumpMemory());
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(memoryDump[i * 2 + STACK_START], i * 2);
    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 32);

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// LDI : Ri = 0

    Interpret.InterpretOne();	// POPALL

    std::vector<Utils::UInt16> registersDump(Cpu.DumpRegisters());
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(registersDump[i], i * 2);
    BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START);

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// PUSH Ri

    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();	// POP Ri

    registersDump = Cpu.DumpRegisters();
    for(int i = 0; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(registersDump[i], (NB_REGISTERS - 1 - i) * 2);
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

    std::vector<UInt16> shiftDump(Cpu.DumpRegisters());

    BOOST_REQUIRE_EQUAL(shiftDump[0], 57344);
    BOOST_REQUIRE_EQUAL(shiftDump[1], 7);
    BOOST_REQUIRE_EQUAL(shiftDump[2], 57344);
    BOOST_REQUIRE_EQUAL(shiftDump[3], 65535);
    BOOST_REQUIRE_EQUAL(shiftDump[4], 65408);
    BOOST_REQUIRE_EQUAL(shiftDump[5], 511);
    BOOST_REQUIRE_EQUAL(shiftDump[6], 65408);
    BOOST_REQUIRE_EQUAL(shiftDump[7], 65535);
    
    for(int i = NB_REGISTERS/2; i < NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(shiftDump[i], 0);
}

BOOST_AUTO_TEST_CASE( RndTest )
{
    Interpret.AcquireProgram(std::move(RndTestData));
    for(int i = 0; i < NB_REGISTERS; ++i)
        Interpret.InterpretOne();

    std::vector<UInt16> rndDump(Interpret.DumpCPUState().DumpRegisters());
    for(int i = 0; i < NB_REGISTERS; ++i)
    {
        BOOST_REQUIRE_GE(rndDump[i], 0);
        BOOST_REQUIRE_LE(rndDump[i], 0xFF00);
    }
}

BOOST_AUTO_TEST_SUITE_END()
