#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserIfTests
#endif
#endif

#include "toslang_parser_fixture.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseIfLiteralCondTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_literal_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = static_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);
    
    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = static_cast<const BooleanExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseIfSimpleCondTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_simple_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = static_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);

    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(cExpr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "BoolVar");

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

BOOST_AUTO_TEST_CASE( ParseIfMultiCondTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_bin_bool_cond.tos");
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

    BOOST_REQUIRE(bStmts[1]->GetKind() == ASTNode::NodeKind::IF_STMT);
    const IfStmt* iStmt = static_cast<const IfStmt*>(bStmts[1].get());
    BOOST_REQUIRE(iStmt != nullptr);

    const Expr* cExpr = iStmt->GetCondExpr();
    BOOST_REQUIRE(cExpr != nullptr);
    BOOST_REQUIRE(cExpr->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bExpr = static_cast<const BinaryOpExpr*>(cExpr);
    BOOST_REQUIRE(bExpr != nullptr);

    const CompoundStmt* iBody = iStmt->GetBody();
    BOOST_REQUIRE(iBody != nullptr);
    auto& bodyStmts = iBody->GetStatements();
    BOOST_REQUIRE_EQUAL(bodyStmts.size(), 1);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadIfNoCondTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_no_cond.tos");
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
    BOOST_REQUIRE_EQUAL(messages.size(), 4);    // TODO: We might need to introduce a ParenExpr...
    //BOOST_REQUIRE_EQUAL(messages[0], "IF ERROR: Missing if condition at line 6, column 2");
}

BOOST_AUTO_TEST_CASE( ParseBadIfNoBodyTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_no_body.tos");
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
    BOOST_REQUIRE_EQUAL(messages[1], "IF ERROR: Missing if body at line 5, column 1");
    BOOST_REQUIRE_EQUAL(messages[2], "SYNTAX ERROR: Expected '}' at line 5, column 1");
}

BOOST_AUTO_TEST_CASE( ParseBadIfGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../sources/if/if_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);
    
    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 2");
}

BOOST_AUTO_TEST_SUITE_END()
