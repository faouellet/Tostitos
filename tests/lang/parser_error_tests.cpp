#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Parser_Error
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

#include "Parse/parser.h"
#include "AST/declarations.h"
#include "AST/expressions.h"

BOOST_FIXTURE_TEST_CASE( BadInitTest, FrontEndErrorFixture )
{
    Parser parser(symTab);
    BOOST_REQUIRE(parser.ParseProgram("../inputs/vardecl.cpp") == nullptr);
    BOOST_REQUIRE(parser.ParseProgram("BadFile.tos") == nullptr);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "FILE ERROR: Wrong file type");
    BOOST_REQUIRE_EQUAL(messages[1], "FILE ERROR: Problem opening the specified file");
}

BOOST_FIXTURE_TEST_CASE( ParseBadVarDeclTest, FrontEndErrorFixture )
{
    Parser parser(symTab);
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/badvardecl.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 8);

	// Check that we have no nullptr. There should only be error nodes with one non-error node
	BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

    // Check that we only have one non-error node
    BOOST_REQUIRE_EQUAL(std::count_if(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node){ return node->GetKind() != ASTNode::ERROR; }), 1);
    
    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 7);
    BOOST_REQUIRE_EQUAL(messages[0], "VAR ERROR: The var keyword should be followed by an identifier at line 1, column 3");
    BOOST_REQUIRE_EQUAL(messages[1], "VAR ERROR: The var keyword should be followed by an identifier at line 2, column 7");
    BOOST_REQUIRE_EQUAL(messages[2], "VAR ERROR: Missing : between a variable and its type at line 3, column 12");
    BOOST_REQUIRE_EQUAL(messages[3], "VAR ERROR: Missing : between a variable and its type at line 4, column 8");
    BOOST_REQUIRE_EQUAL(messages[4], "VAR ERROR: Missing type from variable declaration at line 5, column 8");
    BOOST_REQUIRE_EQUAL(messages[5], "ERROR: Expected a ; at line 7, column 3");
    BOOST_REQUIRE_EQUAL(messages[6], "VAR ERROR: Trying to redefine already define variable at line 9, column 14");
}
