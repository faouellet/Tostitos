#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserIOTests
#endif
#endif

#include "toslang_parser_fixture.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseHelloWorldTest )
{
    auto& cNodes = GetProgramAST("../programs/io/print_string_literal.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 1);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(bStmts[0].get());
    BOOST_REQUIRE(pStmt != nullptr);

    const IdentifierExpr* msg = dynamic_cast<const IdentifierExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(msg != nullptr);
    BOOST_REQUIRE_EQUAL(msg->GetName(), "Hello World!");
}

BOOST_AUTO_TEST_CASE( ParseBasicIOTest )
{
    auto& cNodes = GetProgramAST("../programs/io/io.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 3);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "Message");

    BOOST_REQUIRE(bStmts[1] != nullptr);
    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::SCAN_STMT);
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(bStmts[1].get());
    BOOST_REQUIRE(sStmt != nullptr);

    const IdentifierExpr* sExpr = dynamic_cast<const IdentifierExpr*>(sStmt->GetInput());
    BOOST_REQUIRE(sExpr != nullptr);
    BOOST_REQUIRE_EQUAL(sExpr->GetName(), "Message");

    BOOST_REQUIRE(bStmts[2] != nullptr);
    BOOST_REQUIRE(bStmts[2]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(bStmts[2].get());
    BOOST_REQUIRE(pStmt != nullptr);

    const IdentifierExpr* pMsg = dynamic_cast<const IdentifierExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(pMsg != nullptr);
    BOOST_REQUIRE_EQUAL(pMsg->GetName(), "Message");
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadIOTest )
{
    auto& cNodes = GetProgramAST("../programs/io/bad_io.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 4);

    // Check that we have only error nodes.
    // TODO: Commented out for now, it should be decommented once I fully figure out what should be the PRINT syntax
    // BOOST_REQUIRE(std::all_of(bStmts.begin(), bStmts.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }));

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 4);
    BOOST_REQUIRE_EQUAL(messages[0], "SCAN ERROR: Missing input variable at line 2, column 5");
    BOOST_REQUIRE_EQUAL(messages[1], "ERROR: Not an acceptable use of a binary operation at line 3, column 6");
    BOOST_REQUIRE_EQUAL(messages[2], "SCAN ERROR: Missing input variable at line 3, column 6");
    BOOST_REQUIRE_EQUAL(messages[3], "PRINT ERROR: Wrong message for printing at line 5, column 7");
}

BOOST_AUTO_TEST_CASE( ParseBadIOGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../programs/io/io_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 5");
}

BOOST_AUTO_TEST_SUITE_END()
