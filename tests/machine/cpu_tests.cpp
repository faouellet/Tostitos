#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE CPU
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "cpu.h"

using namespace MachineEngine::ProcessorSpace;

BOOST_AUTO_TEST_CASE( CpuInitTest )
{
    CPU cpu;
    BOOST_REQUIRE_EQUAL(cpu.InitMemory(std::vector<UInt8>()), EMPTY_ROM_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.InitMemory(std::vector<UInt8>(MEMORY_SIZE + 1)), ROM_OVERFLOW_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.InitMemory(std::vector<UInt8>(MEMORY_SIZE-1)), NO_ERROR);
}

BOOST_AUTO_TEST_CASE( CpuStackPtrTest )
{
    CPU cpu;
    BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START);
    BOOST_REQUIRE_EQUAL(cpu.SetStackPointer(STACK_END), NO_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_END);
    BOOST_REQUIRE_EQUAL(cpu.SetStackPointer(STACK_START), NO_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START);
    BOOST_REQUIRE_EQUAL(cpu.SetStackPointer(0), CPU::MEMORY_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START);
}

BOOST_AUTO_TEST_CASE( CpuFlagRegisterTest )
{
    CPU cpu;
    BOOST_REQUIRE_EQUAL(cpu.DumpFlagRegister(), 0);
    cpu.SetFlagRegister(0x04);
    BOOST_REQUIRE_EQUAL(cpu.DumpFlagRegister(), 0x04);
}

BOOST_AUTO_TEST_CASE( CpuProgramCountertTest )
{
    CPU cpu;
    BOOST_REQUIRE_EQUAL(cpu.DumpProgramCounter(), 0);
    BOOST_REQUIRE_EQUAL(cpu.StepBack(), CPU::MEMORY_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpProgramCounter(), 0);
    BOOST_REQUIRE_EQUAL(cpu.SetProgramCounter(STACK_END), CPU::MEMORY_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpProgramCounter(), 0);
    BOOST_REQUIRE_EQUAL(cpu.SetProgramCounter(STACK_START - 1), NO_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpProgramCounter(), STACK_START - 1);
    BOOST_REQUIRE_EQUAL(cpu.StepBack(), NO_ERROR);
    BOOST_REQUIRE_EQUAL(cpu.DumpProgramCounter(), STACK_START - 5);
}

BOOST_AUTO_TEST_CASE( CpuRegistertTest )
{
    CPU cpu;
    for (size_t i = 0; i < CPU::NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(cpu.SetRegister(static_cast<UInt8>(i), static_cast<UInt16>(i)), NO_ERROR);

    BOOST_REQUIRE_EQUAL(cpu.SetRegister(CPU::NB_REGISTERS, 42), CPU::UNKNOWN_REGISTER);
    
    for (size_t i = 0; i < CPU::NB_REGISTERS; ++i)
        BOOST_REQUIRE_EQUAL(cpu.DumpRegister(static_cast<UInt8>(i)), i);
}
