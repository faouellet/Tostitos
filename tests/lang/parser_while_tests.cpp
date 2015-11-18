#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE WhileTests
#endif
#endif

#include "frontend_error_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, FrontEndErrorFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseWhileLiteralCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/while_literal_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseWhileSimpleCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/while_simple_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "BoolVar");

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseWhileMultiCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/while_multi_cond.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& bStmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(bStmts.size(), 2);

    BOOST_REQUIRE(bStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(bStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "BoolVar");

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::WHILE_STMT);
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(bStmts[1].get());
    BOOST_REQUIRE(wStmt != nullptr);

    const Expr* cExpr = wStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(cExpr);

    const CompoundStmt* wBody = wStmt->GetBody();
    BOOST_REQUIRE(wBody != nullptr);
    auto& bodyStmts = wBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadWhileNoCondTest )
{
    BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( ParseBadWhileNoBodyTest )
{
    BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_SUITE_END()
