#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeCheckerArrayTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_sema_fixture.h"

BOOST_FIXTURE_TEST_SUITE( SemaTestSuite, TosLangSemaFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ArrayInitTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/array/array_init_identifier.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 0);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( BadScalarInitWithArrayTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/array/bad_scalar_init_with_array.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to instantiate a scalar variable with an array expression at line 1, column 17");
}

BOOST_AUTO_TEST_CASE( BadArrayInitWithScalarTypeCheck )
{
    size_t errorCount = GetTypeErrors("../asts/array/bad_array_init_with_scalar.ast");
    BOOST_REQUIRE_EQUAL(errorCount, 1);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "TYPE ERROR: Trying to instantiate an array variable with a scalar value at line 1, column 22");
}

BOOST_AUTO_TEST_SUITE_END()
