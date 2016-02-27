#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ISelTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

#include "CodeGen/instructionselector.h"

using namespace TosLang::BackEnd;

BOOST_FIXTURE_TEST_SUITE( BackEndTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( VarInitSelectTest )
{
    BOOST_REQUIRE(true);
    /*auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    size_t errorCount = GetProgramSymbolTable("../programs/var/var_decl.tos", symbolTable);
    BOOST_REQUIRE_EQUAL(errorCount, 0);

    InstructionSelector iSel{ symbolTable };
    InstructionSelector::FunctionGraph fg = iSel.Run(programAST);*/
}

BOOST_AUTO_TEST_SUITE_END()