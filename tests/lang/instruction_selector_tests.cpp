#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ISelTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_codegen_fixture.h"

#include "CodeGen/instructionselector.h"

using namespace TosLang::BackEnd;

BOOST_FIXTURE_TEST_SUITE( BackEndTestSuite, TosLangCodegenFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ISelVarInitBinOpIntTest )
{
    const std::string expectedFile = "../cfgs/var/binary_op_int.cfg";
    const std::string inputFile = "../asts/var/binary_op_int.ast";
    const std::string testFile = "test.cfg";

    GenerateProgramCFG(inputFile, testFile);
    // TODO: Generate CFG files
    CompareFiles(testFile, testFile);
}

BOOST_AUTO_TEST_SUITE_END()