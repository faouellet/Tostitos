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
    auto& cNodes = GetProgramAST("../sources/io/print_string_literal.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 1);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = static_cast<const PrintStmt*>(bStmts[0].get());
    BOOST_REQUIRE(pStmt != nullptr);

    BOOST_REQUIRE(pStmt->GetMessage()->GetKind() == ASTNode::NodeKind::STRING_EXPR);
    const StringExpr* msg = static_cast<const StringExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(msg != nullptr);
    BOOST_REQUIRE_EQUAL(msg->GetName(), "Hello World!");
}

BOOST_AUTO_TEST_CASE( ParseBasicIOTest )
{
    auto& cNodes = GetProgramAST("../sources/io/io.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 3);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "Message");

    BOOST_REQUIRE(bStmts[1] != nullptr);
    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::SCAN_STMT);
    const ScanStmt* sStmt = static_cast<const ScanStmt*>(bStmts[1].get());
    BOOST_REQUIRE(sStmt != nullptr);

    const IdentifierExpr* sExpr = static_cast<const IdentifierExpr*>(sStmt->GetInput());
    BOOST_REQUIRE(sExpr != nullptr);
    BOOST_REQUIRE_EQUAL(sExpr->GetName(), "Message");

    BOOST_REQUIRE(bStmts[2] != nullptr);
    BOOST_REQUIRE(bStmts[2]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = static_cast<const PrintStmt*>(bStmts[2].get());
    BOOST_REQUIRE(pStmt != nullptr);

    BOOST_REQUIRE(pStmt->GetMessage()->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* pMsg = static_cast<const IdentifierExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(pMsg != nullptr);
    BOOST_REQUIRE_EQUAL(pMsg->GetName(), "Message");
}

BOOST_AUTO_TEST_CASE( ParseIOBinOp )
{
    auto& cNodes = GetProgramAST("../sources/io/print_bin_op.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 1);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = static_cast<const PrintStmt*>(bStmts[0].get());
    BOOST_REQUIRE(pStmt != nullptr);

    BOOST_REQUIRE(pStmt->GetMessage()->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* msg = static_cast<const BinaryOpExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(msg != nullptr);
}

BOOST_AUTO_TEST_CASE( ParseIOBoolLiteral )
{
    auto& cNodes = GetProgramAST("../sources/io/print_bool.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 1);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = static_cast<const PrintStmt*>(bStmts[0].get());
    BOOST_REQUIRE(pStmt != nullptr);

    BOOST_REQUIRE(pStmt->GetMessage()->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* msg = static_cast<const BooleanExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(msg != nullptr);
}

BOOST_AUTO_TEST_CASE( ParseIOCall )
{
    auto& cNodes = GetProgramAST("../sources/io/print_call.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::PRINT_STMT);
    const PrintStmt* pStmt = static_cast<const PrintStmt*>(bStmts[0].get());
    BOOST_REQUIRE(pStmt != nullptr);

    BOOST_REQUIRE(pStmt->GetMessage()->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* msg = static_cast<const CallExpr*>(pStmt->GetMessage());
    BOOST_REQUIRE(msg != nullptr);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadIOTest )
{
    auto& cNodes = GetProgramAST("../sources/io/bad_io.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 4);

    // Check that we have only error nodes.
    BOOST_REQUIRE(std::all_of(bStmts.begin(), bStmts.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }));

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 6);
    BOOST_REQUIRE_EQUAL(messages[0], "SCAN ERROR: Missing input variable at line 2, column 5");
    BOOST_REQUIRE_EQUAL(messages[1], "ERROR: Not an acceptable use of a binary operation at line 3, column 6");
    BOOST_REQUIRE_EQUAL(messages[2], "SCAN ERROR: Missing input variable at line 3, column 6");
    BOOST_REQUIRE_EQUAL(messages[3], "SCAN ERROR: Input variable is not an identfier at line 4, column 6");
    BOOST_REQUIRE_EQUAL(messages[4], "ERROR: Not an acceptable use of a binary operation at line 5, column 7");
    BOOST_REQUIRE_EQUAL(messages[5], "PRINT ERROR: Wrong message for printing at line 5, column 7");
}

BOOST_AUTO_TEST_CASE( ParseBadIOGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../sources/io/io_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 5");
}

BOOST_AUTO_TEST_SUITE_END()
