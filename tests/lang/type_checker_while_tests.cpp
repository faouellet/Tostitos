#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerWhileTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( WhileLiteralCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/while/while_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileVariableCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/while/while_simple_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileBooleanBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/while/while_bin_bool_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileNumberBinaryExprCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/while/while_bin_int_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileMixedBinaryExprCondTypeCheck )
{
    //size_t errorCount = GetTypeErrors("../inputs/while/while_bin_bool_cond.tos");
    //BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( WhileCallCondTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/while/while_call_cond.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);    
}

//////////////////// ERROR USE CASES ////////////////////

// TODOs
// Condition is an integer literal
// Condition is an integer variable
// Condition is a call to a function with the wrong return type
// Condition is a binary expression that doesn't produce a boolean value

BOOST_AUTO_TEST_SUITE_END()