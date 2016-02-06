#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ScopeCheckerVarTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BasicCallScopeCheck )
{
    size_t errorCount = GetAccessibilityErrors("../inputs/call/call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( CallChoiceScopeCheck )
{
    size_t errorCount = GetAccessibilityErrors("../inputs/call/call_multiple_choices.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( UndeclaredFunctionCallScopeCheck )
{
    size_t errorCount = GetAccessibilityErrors("../inputs/call/call_undeclared.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Trying to call an undeclared function at line 3, column 26");
}

BOOST_AUTO_TEST_SUITE_END()