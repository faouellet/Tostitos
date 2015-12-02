#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeChecker
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

#include "Parse/parser.h"
#include "Sema/typechecker.h"

BOOST_FIXTURE_TEST_CASE( VarInitTypeError, FrontEndErrorFixture )
{
    /*Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/type_error.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    TypeChecker tChecker(symTab);
    tChecker.TypeCheck(rootNode);

    BOOST_REQUIRE_EQUAL(tChecker.GetErrorCount(), 4);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 4);
    for (int i = 0; i < 3; ++i)
    {
        BOOST_REQUIRE_EQUAL(messages[i], "TYPE ERROR: Trying to instantiate variable with a literal of the wrong type");
    }

    BOOST_REQUIRE_EQUAL(messages.back(), "VAR ERROR: Trying to assign an undeclared variable");*/
}