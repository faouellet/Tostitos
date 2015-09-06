#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE RegAlloc
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "Parse/parser.h"
#include "CodeGen/registerallocator.h"

using namespace TosLang::FrontEnd;

BOOST_AUTO_TEST_CASE( RegAllocTest )
{
    BOOST_REQUIRE(true);
}