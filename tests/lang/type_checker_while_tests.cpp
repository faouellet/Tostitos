#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerWhileTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( WhileLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/while_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/while_simple_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileBooleanBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/while_bin_bool_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/while_bin_int_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileMixedBinaryExprCondTypeCheck )
{
    //size_t errorCount = GetTypeErrors("../programs/while/while_bin_bool_cond.tos");
    //BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/while_call_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);    
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BadWhileNumberLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/bad_while_int_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 7");
}

BOOST_AUTO_TEST_CASE( BadWhileNumberVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/bad_while_int_variable_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 5, column 12");
}

BOOST_AUTO_TEST_CASE( BadWhileNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/bad_while_bin_int_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 4, column 9");
}

BOOST_AUTO_TEST_CASE( BadWhileCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/while/bad_while_call_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Conditional expression must evaluate to a boolean value at line 8, column 9");
}

BOOST_AUTO_TEST_SUITE_END()