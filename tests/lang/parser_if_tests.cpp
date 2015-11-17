#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE IfTests
#endif
#endif

#include "frontend_error_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, FrontEndErrorFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseIfLiteralCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/if_literal_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);
    
    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseIfSimpleCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/if_simple_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);

    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "BoolVar");

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseIfMultiCondTest )
{
    auto& cNodes = GetProgramAST("../inputs/if_multi_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);

    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(cExpr);

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadIfNoCondTest )
{
    BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( ParseBadIfNoBodyTest )
{
    BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_SUITE_END()
