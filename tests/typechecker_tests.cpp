#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE TypeChecker
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "parser.h"
#include "typechecker.h"

#include <iostream>

struct TypeErrorFixture
{
    TypeErrorFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
    }

    ~TypeErrorFixture()
    {
        std::cerr.rdbuf(oldBuffer);
        buffer.clear();
    }

    std::vector<std::string> GetErrorMessages()
    {
        std::vector<std::string> errorMessages;
        std::string message;
        while (std::getline(buffer, message))
            errorMessages.push_back(message);

        return errorMessages;
    }

    std::stringstream buffer;
    std::streambuf* oldBuffer;
};

BOOST_FIXTURE_TEST_CASE( VarInitTypeError, TypeErrorFixture )
{
    Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/typeerror.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    TypeChecker tChecker;
    tChecker.TypeCheck(rootNode);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 3);
    for (auto& m : messages)
    {
        BOOST_REQUIRE_EQUAL(m, "TYPE ERROR: Trying to instantiate variable with a literal of the wrong type");
    }
}