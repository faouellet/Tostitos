#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserCallTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

#include <sstream>

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, FrontEndErrorFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseCallZeroArgTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "nothing");

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

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = dynamic_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetArguments();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 2);

    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(mainStmts[0].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 0);

    const ReturnStmt* mainRStmt = dynamic_cast<const ReturnStmt*>(mainStmts[1].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallOneArgTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_one_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "identity");

    const ParamVarDecls* paramDecls = fDecl->GetArguments();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 1);

    const VarDecl* arg = dynamic_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg != nullptr);
    BOOST_REQUIRE_EQUAL(arg->GetName(), "i");

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
    BOOST_REQUIRE_EQUAL(iValue->GetName(), "i");

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = dynamic_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetArguments();
    BOOST_REQUIRE(mainParams != nullptr);
    BOOST_REQUIRE_EQUAL(mainParams->GetParameters().size(), 0);

    const CompoundStmt* mainBody = mainDecl->GetBody();
    BOOST_REQUIRE(mainBody != nullptr);
    auto& mainStmts = mainBody->GetStatements();
    BOOST_REQUIRE_EQUAL(mainStmts.size(), 3);

    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(mainStmts[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyInt");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);

    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(mainStmts[1].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 1);

    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(cArgs[0].get());
    BOOST_REQUIRE(iExpr != nullptr);
    BOOST_REQUIRE_EQUAL(iExpr->GetName(), "MyInt");

    const ReturnStmt* mainRStmt = dynamic_cast<const ReturnStmt*>(mainStmts[2].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseCallMultiArgsTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_multi_args.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "sum");

    const ParamVarDecls* paramDecls = fDecl->GetArguments();
    BOOST_REQUIRE(paramDecls != nullptr);
    auto& params = paramDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        const VarDecl* arg = dynamic_cast<const VarDecl*>(params[i].get());
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

    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    const BinaryOpExpr* bValue = dynamic_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(bValue != nullptr);

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* mainDecl = dynamic_cast<const FunctionDecl*>(cNodes[1].get());
    BOOST_REQUIRE(mainDecl != nullptr);
    BOOST_REQUIRE_EQUAL(mainDecl->GetFunctionName(), "main");

    const ParamVarDecls* mainParams = mainDecl->GetArguments();
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
        const VarDecl* vDecl = dynamic_cast<const VarDecl*>(mainStmts[i].get());
        BOOST_REQUIRE(vDecl != nullptr);
        BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), sStream.str());
        BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), i + 1);
    }

    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(mainStmts[3].get());
    BOOST_REQUIRE(cExpr != nullptr);
    auto& cArgs = cExpr->GetArgs();
    BOOST_REQUIRE_EQUAL(cArgs.size(), 3);

    for (size_t i = 0; i < 3; i++)
    {
        const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(cArgs[i].get());
        BOOST_REQUIRE(iExpr != nullptr);
        std::stringstream sStream;
        sStream << "MyInt";
        sStream << i + 1;
        BOOST_REQUIRE_EQUAL(iExpr->GetName(), sStream.str());
    }

    const ReturnStmt* mainRStmt = dynamic_cast<const ReturnStmt*>(mainStmts[4].get());
    BOOST_REQUIRE(mainRStmt != nullptr);
    const Expr* mainRValue = mainRStmt->GetReturnExpr();
    BOOST_REQUIRE(mainRValue == nullptr);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadCallMissingCommaTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_missing_comma.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
    
    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Not an acceptable binary operation at line 9, column 15");
    BOOST_REQUIRE_EQUAL(messages[1], "CALL ERROR: Incorrect function call arguments at line 9, column 15");
}

BOOST_AUTO_TEST_CASE( ParseBadCallMissingParenTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_missing_paren.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "CALL ERROR: Function call is missing a closing parenthesis at line 9, column 15");
}

BOOST_AUTO_TEST_CASE( ParseBadCallGlobalScopeTest )
{
    auto& cNodes = GetProgramAST("../inputs/call/call_global_scope.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a declaration at line 1, column 3");
}

BOOST_AUTO_TEST_SUITE_END()
