#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerIfTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( SemaTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( IfLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/if_literal_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/if_simple_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfBooleanBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/if_bin_bool_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/if_bin_int_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfMixedBinaryExprCondTypeCheck )
{
    //size_t errorCount = GetTypeErrors("../asts/if/if_bin_bool_cond.ast");
    //BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( IfCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/if_call_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);    
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BadIfNumberLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/bad_if_int_literal_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 4");
}

BOOST_AUTO_TEST_CASE( BadIfNumberVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/bad_if_int_variable_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 5, column 9");
}

BOOST_AUTO_TEST_CASE( BadIfNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/bad_if_bin_int_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 6");
}

BOOST_AUTO_TEST_CASE( BadIfCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/if/bad_if_call_cond.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 2);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: No function matches the expected return type at line 8, column 6");
    BOOST_REQUIRE_EQUAL(messages[1], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 8, column 6");
}

BOOST_AUTO_TEST_SUITE_END()