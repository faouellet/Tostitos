#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerVarTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( VarInitValueBoolTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/var_init_bool.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitValueIdentifierTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/var_init_identifier.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitValueIntTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/var_init_int.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitValueStringTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/var_init_string.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitBinaryOpBoolTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/binary_op_bool.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitBinaryOpIntTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/binary_op_int.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( VarInitCallTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/call/call_one_arg.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

// TODO
// Initialize a variable with a literal of wrong type
// Initialize a variable with a variable of wrong type
// Initialize a variable with a call to a function with the wrong return type
// Initialize a variable with a binary expression producing a value of the wrong type

BOOST_AUTO_TEST_SUITE_END()
