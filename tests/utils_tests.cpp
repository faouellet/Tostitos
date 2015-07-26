#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Utils
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "utils.h"

#include <limits>

using namespace MachineEngine::ProcessorSpace;

BOOST_AUTO_TEST_CASE( StaticConditionalTest )
{
	BOOST_REQUIRE(sizeof(Utils::IfElse<true, char, double>::type) == sizeof(char));
	BOOST_REQUIRE(sizeof(Utils::IfElse<false, char, double>::type) == sizeof(double));
}

BOOST_AUTO_TEST_CASE( TypeSizeTest )
{
	// Test number of bytes
	BOOST_REQUIRE_LE(sizeof(Utils::UInt_<1>), 1);
	BOOST_REQUIRE_LE(sizeof(Utils::UInt_<2>), 2);
	BOOST_REQUIRE_LE(sizeof(Utils::UInt_<4>), 4);
	BOOST_REQUIRE_LE(sizeof(Utils::UInt_<8>), 8);

	// Test sign
	BOOST_REQUIRE_EQUAL(std::numeric_limits<Utils::UInt_<1>::type>::min(), 0);
	BOOST_REQUIRE_EQUAL(std::numeric_limits<Utils::UInt_<2>::type>::min(), 0);
	BOOST_REQUIRE_EQUAL(std::numeric_limits<Utils::UInt_<4>::type>::min(), 0);
	BOOST_REQUIRE_EQUAL(std::numeric_limits<Utils::UInt_<8>::type>::min(), 0);
		
	// Test number of bytes
	BOOST_REQUIRE_LE(sizeof(Utils::Int_<1>), 1);
	BOOST_REQUIRE_LE(sizeof(Utils::Int_<2>), 2);
	BOOST_REQUIRE_LE(sizeof(Utils::Int_<4>), 4);
	BOOST_REQUIRE_LE(sizeof(Utils::Int_<8>), 8);
		
	// Test sign
	BOOST_REQUIRE_LT(std::numeric_limits<Utils::Int_<1>::type>::min(), 0);
	BOOST_REQUIRE_LT(std::numeric_limits<Utils::Int_<2>::type>::min(), 0);
	BOOST_REQUIRE_LT(std::numeric_limits<Utils::Int_<4>::type>::min(), 0);
	BOOST_REQUIRE_LT(std::numeric_limits<Utils::Int_<8>::type>::min(), 0);

	// Test error
	BOOST_REQUIRE_EQUAL(sizeof(Utils::Int_<9>), 1);
	BOOST_REQUIRE_EQUAL(sizeof(Utils::UInt_<9>), 1);
}

