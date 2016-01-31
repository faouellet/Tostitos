#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeChecker
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

BOOST_AUTO_TEST_CASE( VarInitValueStrignTypeCheck )
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

/*BOOST_AUTO_TEST_CASE( VarInitCallTypeCheck )
{
    size_t errorCount = GetTypeErrors("");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}*/

//////////////////// ERROR USE CASES ////////////////////

/*BOOST_AUTO_TEST_CASE( VarInitValueTypeError )
{
    size_t errorCount = GetTypeErrors("");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "");
}

BOOST_AUTO_TEST_CASE( VarInitBinaryTypeError )
{
    size_t errorCount = GetTypeErrors("");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "");
}

BOOST_AUTO_TEST_CASE( VarInitCallTypeError )
{
    size_t errorCount = GetTypeErrors("");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "");
}*/

BOOST_AUTO_TEST_SUITE_END()
