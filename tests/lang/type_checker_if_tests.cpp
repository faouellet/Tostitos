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

// TODOs
// Condition is an integer literal
// Condition is an integer variable
// Condition is a call to a function with the wrong return type
// Condition is a binary expression that doesn't produce a boolean value

BOOST_AUTO_TEST_SUITE_END()