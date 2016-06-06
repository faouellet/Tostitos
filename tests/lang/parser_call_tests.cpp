#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserCallTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_parser_fixture.h"

#include <sstream>

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseCallZeroArgTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "nothing");

    const ParamVarDecls* params = fDecl->GetParametersDecl();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParameters().size(), 0);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue == nullptr);

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 2);

    BOOST_REQUIRE(mainStmts[0] != nullptr);
    BOOST_REQUIRE(mainStmts[0]->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(mainStmts[0].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 0);

    BOOST_REQUIRE(mainStmts[1] != nullptr);
    BOOST_REQUIRE(mainStmts[1]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[1].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallOneArgTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_one_arg_var.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "identity");

    const ParamVarDecls* paramDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 1);

    BOOST_REQUIRE(params[0] != nullptr);
    BOOST_REQUIRE(params[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg = static_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg != nullptr);
    BOOST_REQUIRE_EQUAL(arg->GetName(), "i");

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iValue = static_cast<const IdentifierExpr*>(rValue);
    BOOST_REQUIRE(iValue != nullptr);
    BOOST_REQUIRE_EQUAL(iValue->GetName(), "i");

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 3);

    BOOST_REQUIRE(mainStmts[0] != nullptr);
    BOOST_REQUIRE(mainStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(mainStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyInt");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
    const NumberExpr* nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);

    BOOST_REQUIRE(mainStmts[1] != nullptr);
    BOOST_REQUIRE(mainStmts[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = static_cast<const VarDecl*>(mainStmts[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyInt2");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 1);

    BOOST_REQUIRE(cArgs[0] != nullptr);
    BOOST_REQUIRE(cArgs[0]->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(cArgs[0].get());
    BOOST_REQUIRE(iExpr != nullptr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "MyInt");

    BOOST_REQUIRE(mainStmts[2] != nullptr);
    BOOST_REQUIRE(mainStmts[2]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[2].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallOneArgBinopTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_one_arg_binop.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "identity");

    const ParamVarDecls* paramDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 1);

    BOOST_REQUIRE(params[0] != nullptr);
    BOOST_REQUIRE(params[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg = static_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg != nullptr);
    BOOST_REQUIRE_EQUAL(arg->GetName(), "i");

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iValue = static_cast<const IdentifierExpr*>(rValue);
    BOOST_REQUIRE(iValue != nullptr);
    BOOST_REQUIRE_EQUAL(iValue->GetName(), "i");

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 4);

    BOOST_REQUIRE(mainStmts[0] != nullptr);
    BOOST_REQUIRE(mainStmts[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(mainStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyInt1");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
    const NumberExpr* nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 1);

    BOOST_REQUIRE(mainStmts[1] != nullptr);
    BOOST_REQUIRE(mainStmts[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = static_cast<const VarDecl*>(mainStmts[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyInt2");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
    nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 2);

    BOOST_REQUIRE(mainStmts[2] != nullptr);
    BOOST_REQUIRE(mainStmts[2]->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(mainStmts[2].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 1);

    BOOST_REQUIRE(cArgs[0] != nullptr);
    BOOST_REQUIRE(cArgs[0]->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* binExpr = static_cast<const BinaryOpExpr*>(cArgs[0].get());
    BOOST_REQUIRE(binExpr != nullptr);

    BOOST_REQUIRE(mainStmts[3] != nullptr);
    BOOST_REQUIRE(mainStmts[3]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[3].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallMultiArgsTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_multi_args.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "sum");

    const ParamVarDecls* paramDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        BOOST_REQUIRE(params[i] != nullptr);
        BOOST_REQUIRE(params[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* arg = static_cast<const VarDecl*>(params[i].get());
        BOOST_REQUIRE(arg != nullptr);
        std::stringstream sStream;
        sStream << "i";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(arg->GetName(), sStream.str());
    }

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bValue = static_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(bValue != nullptr);

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 5);

    for (size_t i = 0; i < 3; i++)
    {
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE(mainStmts[i] != nullptr);
        BOOST_REQUIRE(mainStmts[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* vDecl = static_cast<const VarDecl*>(mainStmts[i].get());
        BOOST_REQUIRE(vDecl != nullptr);
        BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), sStream.str());
        BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
        const NumberExpr* nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), i + 1);
    }

    BOOST_REQUIRE(mainStmts[3] != nullptr);
    BOOST_REQUIRE(mainStmts[3]->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(mainStmts[3].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        BOOST_REQUIRE(cArgs[i] != nullptr);
        BOOST_REQUIRE(cArgs[i]->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
        const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(cArgs[i].get());
        BOOST_REQUIRE(iExpr != nullptr);
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(iExpr->GetName(), sStream.str());
    }

    BOOST_REQUIRE(mainStmts[4] != nullptr);
    BOOST_REQUIRE(mainStmts[4]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[4].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallMultiArgsOneBinopTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_multi_args_one_binop.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "sum");

    const ParamVarDecls* paramDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        BOOST_REQUIRE(params[i] != nullptr);
        BOOST_REQUIRE(params[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* arg = static_cast<const VarDecl*>(params[i].get());
        BOOST_REQUIRE(arg != nullptr);
        std::stringstream sStream;
        sStream << "i";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(arg->GetName(), sStream.str());
    }

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bValue = static_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(bValue != nullptr);

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 5);

    for (size_t i = 0; i < 3; i++)
    {
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE(mainStmts[i] != nullptr);
        BOOST_REQUIRE(mainStmts[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* vDecl = static_cast<const VarDecl*>(mainStmts[i].get());
        BOOST_REQUIRE(vDecl != nullptr);
        BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), sStream.str());
        BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
        const NumberExpr* nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), i + 1);
    }

    BOOST_REQUIRE(mainStmts[3] != nullptr);
    BOOST_REQUIRE(mainStmts[3]->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(mainStmts[3].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 3);

    BOOST_REQUIRE(cArgs[0] != nullptr);
    BOOST_REQUIRE(cArgs[0]->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* binExpr = static_cast<const BinaryOpExpr*>(cArgs[0].get());
    BOOST_REQUIRE(binExpr != nullptr);

    for (size_t i = 1; i < 3; i++)
    {
        BOOST_REQUIRE(cArgs[i] != nullptr);
        BOOST_REQUIRE(cArgs[i]->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
        const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(cArgs[i].get());
        BOOST_REQUIRE(iExpr != nullptr);
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(iExpr->GetName(), sStream.str());
    }

    BOOST_REQUIRE(mainStmts[4] != nullptr);
    BOOST_REQUIRE(mainStmts[4]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[4].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallMultiArgsMultiBinopsTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_multi_args_multi_binops.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "sum");

    const ParamVarDecls* paramDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        BOOST_REQUIRE(params[i] != nullptr);
        BOOST_REQUIRE(params[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* arg = static_cast<const VarDecl*>(params[i].get());
        BOOST_REQUIRE(arg != nullptr);
        std::stringstream sStream;
        sStream << "i";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(arg->GetName(), sStream.str());
    }

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0] != nullptr);
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
    const BinaryOpExpr* bValue = static_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(bValue != nullptr);

    BOOST_REQUIRE(cNodes[1] != nullptr);
    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = static_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetParametersDecl();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 5);

    for (size_t i = 0; i < 3; i++)
    {
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE(mainStmts[i] != nullptr);
        BOOST_REQUIRE(mainStmts[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* vDecl = static_cast<const VarDecl*>(mainStmts[i].get());
        BOOST_REQUIRE(vDecl != nullptr);
        BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), sStream.str());
        BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
        const NumberExpr* nExpr = static_cast<const NumberExpr*>(vDecl->GetInitExpr());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), i + 1);
    }

    BOOST_REQUIRE(mainStmts[3] != nullptr);
    BOOST_REQUIRE(mainStmts[3]->GetKind() == ASTNode::NodeKind::CALL_EXPR);
    const CallExpr* cExpr = static_cast<const CallExpr*>(mainStmts[3].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 3);

    for (size_t i = 1; i < 3; i++)
    {
        BOOST_REQUIRE(cArgs[i] != nullptr);
        BOOST_REQUIRE(cArgs[i]->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
        const BinaryOpExpr* binExpr = static_cast<const BinaryOpExpr*>(cArgs[i].get());
        BOOST_REQUIRE(binExpr != nullptr);
    }

    BOOST_REQUIRE(mainStmts[4] != nullptr);
    BOOST_REQUIRE(mainStmts[4]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* mainRStmt = static_cast<const ReturnStmt*>(mainStmts[4].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadCallMissingCommaTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_missing_comma.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    
    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Not an acceptable binary operation at line 9, column 15");
    BOOST_REQUIRE_EQUAL(messages[1], "CALL ERROR: Incorrect function call arguments at line 9, column 15");
}

BOOST_AUTO_TEST_CASE( ParseBadCallMissingParenTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_missing_paren.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Function call is missing a closing parenthesis at line 9, column 15");
}

BOOST_AUTO_TEST_CASE( ParseBadCallGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../sources/call/call_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 3");
}

BOOST_AUTO_TEST_SUITE_END()
