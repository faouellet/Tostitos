#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Instruction
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "instruction.h"

#include <vector>

using namespace MachineEngine::ProcessorSpace;

BOOST_AUTO_TEST_CASE( InstructionGetSetTest )
{
    const unsigned addInst = 0x52000000;
    Instruction inst(addInst);
    BOOST_REQUIRE(!inst.UseImmediateValue());
    //BOOST_REQUIRE_EQUAL(inst.GetType(), Instruction::ADD);
	BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(inst.UseSecondOperand());
	BOOST_REQUIRE(inst.UseThirdOperand());

    inst.SetFirstOperand(1);
    BOOST_REQUIRE_EQUAL(inst.GetFirstOperand(), 1);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(!inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(inst.UseSecondOperand());
	BOOST_REQUIRE(inst.UseThirdOperand());

    inst.SetSecondOperand(2);
    BOOST_REQUIRE_EQUAL(inst.GetSecondOperand(), 2);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(!inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(inst.UseSecondOperand());
	BOOST_REQUIRE(inst.UseThirdOperand());

    inst.SetThirdOperand(3);
    BOOST_REQUIRE_EQUAL(inst.GetThirdOperand(), 3);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(!inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(inst.UseSecondOperand());
	BOOST_REQUIRE(inst.UseThirdOperand());

    inst.SetImmediateValue(4);
    BOOST_REQUIRE_EQUAL(inst.GetImmediateValue(), 3);   // Value of the third register since we don't use an immediate value
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(!inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(inst.UseSecondOperand());
	BOOST_REQUIRE(inst.UseThirdOperand());
}

BOOST_AUTO_TEST_CASE( InstructionGetSetImmediateTest )
{
    const unsigned addImmInst = 0x50000000;
    Instruction inst(addImmInst);
    BOOST_REQUIRE(inst.UseImmediateValue());
    //BOOST_REQUIRE_EQUAL(inst.GetType(), Instruction::ADD_IMM);
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(!inst.UseSecondOperand());
	BOOST_REQUIRE(!inst.UseThirdOperand());

    inst.SetFirstOperand(1);
    BOOST_REQUIRE_EQUAL(inst.GetFirstOperand(), 1);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(!inst.UseSecondOperand());
	BOOST_REQUIRE(!inst.UseThirdOperand());

    inst.SetSecondOperand(2);
    BOOST_REQUIRE_EQUAL(inst.GetSecondOperand(), 0);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(!inst.UseSecondOperand());
	BOOST_REQUIRE(!inst.UseThirdOperand());

    inst.SetThirdOperand(3);
    BOOST_REQUIRE_EQUAL(inst.GetThirdOperand(), 0);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(!inst.UseSecondOperand());
	BOOST_REQUIRE(!inst.UseThirdOperand());

    inst.SetImmediateValue(4);
    BOOST_REQUIRE_EQUAL(inst.GetImmediateValue(), 4);
    BOOST_REQUIRE(!inst.IsInplace());
    BOOST_REQUIRE(inst.UseImmediateValue());
    BOOST_REQUIRE(inst.IsArithmeticInstruction());
	BOOST_REQUIRE(!inst.UseSecondOperand());
	BOOST_REQUIRE(!inst.UseThirdOperand());
}
