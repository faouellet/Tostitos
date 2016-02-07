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
    size_t errorCount = GetTypeErrors("../inputs/call/call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

// TODO
// Initialize a variable with a call to a function with the wrong return type

BOOST_AUTO_TEST_CASE( BadVarInitBinaryOpTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/bad_var_init_bin_op.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to instantiate variable with an expression of the wrong type at line 1, column 20");
}

BOOST_AUTO_TEST_CASE( BadVarInitCallTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/call/bad_return_call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to initialize a variable with the return value of a function of the wrong type at line 6, column 15");
}

BOOST_AUTO_TEST_CASE( BadVarInitIdentifierTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/bad_var_init_identifier.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to initialize a variable with another variable of a different type at line 2, column 12");
}

BOOST_AUTO_TEST_CASE( BadVarInitLiteralTypeCheck )
{
    size_t errorCount = GetTypeErrors("../inputs/var/bad_var_init_literal.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to instantiate variable with a literal of the wrong type at line 1, column 16");
}

BOOST_AUTO_TEST_SUITE_END()
