#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerIfTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( IfLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/if_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/if_simple_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfBooleanBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/if_bin_bool_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/if_bin_int_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfMixedBinaryExprCondTypeCheck )
{
    //size_t errorCount = GetTypeErrors("../inputs/if/if_bin_bool_cond.tos");
    //BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/if_call_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);    
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BadIfNumberLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/bad_if_int_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 4");
}

BOOST_AUTO_TEST_CASE( BadIfNumberVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/bad_if_int_variable_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 5, column 9");
}

BOOST_AUTO_TEST_CASE( BadIfNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/bad_if_bin_int_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 6");
}

BOOST_AUTO_TEST_CASE( BadIfCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/if/bad_if_call_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 8, column 6");
}

BOOST_AUTO_TEST_SUITE_END()