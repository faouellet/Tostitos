#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerIOTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( SemaTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( PrintNumberLiteralTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/io/print_number_literal.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( PrintNumberVariableTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/io/print_number_variable.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( PrintStringLiteralTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/io/print_string_literal.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

BOOST_AUTO_TEST_CASE( PrintStringVariableTypeCheck )
{
    size_t errorCount = GetTypeErrors("../programs/io/print_string_variable.tos");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_SUITE_END()
