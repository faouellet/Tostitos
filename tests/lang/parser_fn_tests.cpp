#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserFnTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_parser_fixture.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseFuncVoidTest )
{
    auto& cNodes = GetProgramAST("../sources/function/fn_def_void.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* params = fDecl->GetParametersDecl();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParameters().size(), 0);

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseFuncZeroArgTest )
{
    auto& cNodes = GetProgramAST("../sources/function/fn_def_zero_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");
    
    const ParamVarDecls* params = fDecl->GetParametersDecl();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParameters().size(), 0);
    
    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);
    
    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
    const NumberExpr* nValue = static_cast<const NumberExpr*>(rValue);
    BOOST_REQUIRE(nValue != nullptr);
    BOOST_REQUIRE_EQUAL(nValue->GetValue(), 42);
}

BOOST_AUTO_TEST_CASE( ParseFuncOneArgTest )
{
    auto& cNodes = GetProgramAST("../sources/function/fn_def_one_arg.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* pVDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(pVDecls != nullptr);
    auto& params = pVDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 1);
    BOOST_REQUIRE(params[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg = static_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg != nullptr);
    BOOST_REQUIRE_EQUAL(arg->GetName(), "arg");
    BOOST_REQUIRE(arg->IsFunctionParameter());

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    BOOST_REQUIRE(rValue->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    const IdentifierExpr* iValue = static_cast<const IdentifierExpr*>(rValue);
    BOOST_REQUIRE(iValue != nullptr);
    BOOST_REQUIRE_EQUAL(iValue->GetName(), "arg");
}

BOOST_AUTO_TEST_CASE( ParseFuncMultiArgsTest )
{
    auto& cNodes = GetProgramAST("../sources/function/fn_def_multi_args.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = static_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");

    const ParamVarDecls* pVDecls = fDecl->GetParametersDecl();
    BOOST_REQUIRE(pVDecls != nullptr);
    auto& params = pVDecls->GetParameters();
    BOOST_REQUIRE_EQUAL(params.size(), 3);
    
    BOOST_REQUIRE(params[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg1 = static_cast<const VarDecl*>(params[0].get());
    BOOST_REQUIRE(arg1 != nullptr);
    BOOST_REQUIRE_EQUAL(arg1->GetName(), "arg1");
    BOOST_REQUIRE(arg1->IsFunctionParameter());

    BOOST_REQUIRE(params[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg2 = static_cast<const VarDecl*>(params[1].get());
    BOOST_REQUIRE(arg2 != nullptr);
    BOOST_REQUIRE_EQUAL(arg2->GetName(), "arg2");
    BOOST_REQUIRE(arg2->IsFunctionParameter());

    BOOST_REQUIRE(params[2]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* arg3 = static_cast<const VarDecl*>(params[2].get());
    BOOST_REQUIRE(arg3 != nullptr);
    BOOST_REQUIRE_EQUAL(arg3->GetName(), "arg3");
    BOOST_REQUIRE(arg3->IsFunctionParameter());

    const CompoundStmt* body = fDecl->GetBody();
    BOOST_REQUIRE(body != nullptr);
    auto& stmts = body->GetStatements();
    BOOST_REQUIRE_EQUAL(stmts.size(), 1);

    BOOST_REQUIRE(stmts[0]->GetKind() == ASTNode::NodeKind::RETURN_STMT);
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(stmts[0].get());
    BOOST_REQUIRE(rStmt != nullptr);
    const Expr* rValue = rStmt->GetReturnExpr();
    BOOST_REQUIRE(rValue != nullptr);
    const BinaryOpExpr* rExpr = static_cast<const BinaryOpExpr*>(rValue);
    BOOST_REQUIRE(rExpr != nullptr);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingArrow )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_arrow.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);
    
    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Missing arrow to return type at line 1, column 11");
}

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingName )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_no_name.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: The fn keyword should be followed by an identifier at line 1, column 3");
}

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingLeftParen )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_left_paren.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Missing left parenthesis in the function declaration at line 1, column 8");
}

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingRightParen )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_right_paren.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Missing right parenthesis in the function declaration at line 1, column 13");
}

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingReturnType )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_return_type.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Missing return type at line 1, column 10");
}

BOOST_AUTO_TEST_CASE( ParseBadFunctionMissingInternalFunction )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_internal_fn.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "FUNCTION ERROR: Internal functions are not allowed at line 2, column 3");
}

BOOST_AUTO_TEST_CASE( ParseBadParamMissingName )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_param_name.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "PARAM ERROR: Expected parameter name at line 1, column 8");
}

BOOST_AUTO_TEST_CASE( ParseBadParamMissingType )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_param_type.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "PARAM ERROR: Expected parameter type at line 1, column 10");
}

BOOST_AUTO_TEST_CASE( ParseBadParamMissingColon )
{
    auto& cNodes = GetProgramAST("../sources/function/bad_fn_missing_param_colon.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    auto& node = cNodes.front();
    BOOST_REQUIRE(node != nullptr);
    BOOST_REQUIRE(node->GetKind() == ASTNode::NodeKind::ERROR);

    // Check if the correct error message got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "PARAM ERROR: Missing colon between a parameter and its type at line 1, column 11");
}

BOOST_AUTO_TEST_SUITE_END()
