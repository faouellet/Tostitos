#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserFnTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, FrontEndErrorFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseFuncVoidTest )
{
    auto& cNodes = GetProgramAST("../inputs/function/fn_def_void.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* params = fDecl->GetArguments();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParameters().size(), 0);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseFuncZeroArgTest )
{
    auto& cNodes = GetProgramAST("../inputs/function/fn_def_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");
    
    const ParamVarDecls* params = fDecl->GetArguments();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParameters().size(), 0);
    
    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);
    
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    const NumberExpr* nValue = dynamic_cast<const NumberExpr*>(rValue);
    BOOST_REQUIRE(nValue != nullptr);
    BOOST_REQUIRE_EQUAL(nValue->GetValue(), 42);
}

BOOST_AUTO_TEST_CASE( ParseFuncOneArgTest )
{
    auto& cNodes = GetProgramAST("../inputs/function/fn_def_one_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* pVDecls = fDecl->GetArguments();
    BOOST_REQUIRE(pVDecls != nullptr);
    auto& params = pVDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 1);
    const VarDecl* arg = dynamic_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg != nullptr);
    BOOST_REQUIRE_EQUAL(arg->GetName(), "arg");

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    const IdentifierExpr* iValue = dynamic_cast<const IdentifierExpr*>(rValue);
    BOOST_REQUIRE(iValue != nullptr);
    BOOST_REQUIRE_EQUAL(iValue->GetName(), "arg");
}

BOOST_AUTO_TEST_CASE( ParseFuncMultiArgsTest )
{
    auto& cNodes = GetProgramAST("../inputs/function/fn_def_multi_args.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* pVDecls = fDecl->GetArguments();
    BOOST_REQUIRE(pVDecls != nullptr);
    auto& params = pVDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);
    
    const VarDecl* arg1 = dynamic_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg1 != nullptr);
    BOOST_REQUIRE_EQUAL(arg1->GetName(), "arg1");

    const VarDecl* arg2 = dynamic_cast<const VarDecl*>(params[1].get());
    BOOST_REQUIRE(arg2 != nullptr);
    BOOST_REQUIRE_EQUAL(arg2->GetName(), "arg2");

    const VarDecl* arg3 = dynamic_cast<const VarDecl*>(params[2].get());
    BOOST_REQUIRE(arg3 != nullptr);
    BOOST_REQUIRE_EQUAL(arg3->GetName(), "arg3");

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    const BinaryOpExpr* rExpr = dynamic_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(rExpr != nullptr);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_SUITE_END()
