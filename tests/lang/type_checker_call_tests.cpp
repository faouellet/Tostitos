#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerCallTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( SemaTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( CallBinOpArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/call_one_arg_binop.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( CallCallArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/call_one_arg_call.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( CallLiteralArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/call_one_arg_literal.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( CallVariableArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( CallNoArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/call_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BadCallBinOpArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/bad_call_one_arg_binop.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Trying to call a function with a parameter of the wrong type at line 9, column 21");
}

BOOST_AUTO_TEST_CASE( BadCallCallArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/bad_call_one_arg_call.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Trying to call a function with a parameter of the wrong type at line 7, column 33");
}

BOOST_AUTO_TEST_CASE( BadCallLiteralArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/bad_call_one_arg_literal.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Trying to call a function with a parameter of the wrong type at line 6, column 25");
}

BOOST_AUTO_TEST_CASE( BadCallVariableArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/bad_param_call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Trying to call a function with a parameter of the wrong type at line 7, column 26");
}

BOOST_AUTO_TEST_CASE( BadCallNoArgTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/call/bad_call_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Trying to call a function with the wrong number of arguments at line 6, column 8");
}

BOOST_AUTO_TEST_SUITE_END()