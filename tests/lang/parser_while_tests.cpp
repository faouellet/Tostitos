#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE WhileTests
#endif
#endif

#include "toslang_parser_fixture.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseWhileLiteralCondTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1] != nullptr);
    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = static_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = static_cast<const BooleanExpr*>(cExpr);
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseWhileSimpleCondTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_simple_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0] != nullptr);
    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1] != nullptr);
    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = static_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "BoolVar");

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseWhileMultiCondTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_bin_bool_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = static_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bExpr = static_cast<const BinaryOpExpr*>(cExpr);
    BOOST_REQUIRE(bExpr != nullptr);

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadWhileNoCondTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_no_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 3);

    // Check that we only have one error statement
    BOOST_REQUIRE_EQUAL(std::count_if(bStmts.begin(), bStmts.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }), 1);

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 4); // TODO: We might need to introduce a ParenExpr...
    //BOOST_REQUIRE_EQUAL(messages[0], "WHILE ERROR: Missing while condition at line 6, column 2");
}

BOOST_AUTO_TEST_CASE( ParseBadWhileNoBodyTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_no_body.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    // Check that we only have one error statement
    BOOST_REQUIRE_EQUAL(std::count_if(bStmts.begin(), bStmts.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }), 1);

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 3);
    BOOST_REQUIRE_EQUAL(messages[0], "SYNTAX ERROR: Expected '{' at line 5, column 1");
    BOOST_REQUIRE_EQUAL(messages[1], "WHILE ERROR: Missing while body at line 5, column 1");
    BOOST_REQUIRE_EQUAL(messages[2], "SYNTAX ERROR: Expected '}' at line 5, column 1");
}

BOOST_AUTO_TEST_CASE( ParseBadWhileGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../sources/while/while_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 5");
}

BOOST_AUTO_TEST_SUITE_END()
