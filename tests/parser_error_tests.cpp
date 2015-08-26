#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Parser_Error
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "console_error_fixture.h"
#include "parser.h"
#include "declarations.h"
#include "expressions.h"

BOOST_FIXTURE_TEST_CASE( BadInitTest, ConsoleErrorFixture )
{
    Parser parser;
    BOOST_REQUIRE(parser.ParseProgram("../inputs/vardecl.cpp") == nullptr);
    BOOST_REQUIRE(parser.ParseProgram("BadFile.tos") == nullptr);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE(messages[0] == "FILE ERROR: Wrong file type");
    BOOST_REQUIRE(messages[1] == "FILE ERROR: Problem opening the specified file");
}

BOOST_FIXTURE_TEST_CASE( ParseBadVarDeclTest, ConsoleErrorFixture )
{
    Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/badvardecl.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 6);

    // Check if we only have error nodes
    for (auto& node : cNodes)
    {
        BOOST_REQUIRE(node != nullptr);
        BOOST_REQUIRE(node->GetKind() == ASTNode::ERROR);
    }
    
    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 6);
    BOOST_REQUIRE(messages[0] == "VAR ERROR: The var keyword should be followed by an identifier");
    BOOST_REQUIRE(messages[1] == "VAR ERROR: The var keyword should be followed by an identifier");
    BOOST_REQUIRE(messages[2] == "VAR ERROR: Missing : between a variable and its type");
    BOOST_REQUIRE(messages[3] == "VAR ERROR: Missing : between a variable and its type");
    BOOST_REQUIRE(messages[4] == "VAR ERROR: Missing type from variable declaration");
    BOOST_REQUIRE(messages[5] == "ERROR: Expected a ;");
}
