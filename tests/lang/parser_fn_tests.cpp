#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserVarTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

#include "Parse/parser.h"
#include "AST/declarations.h"
#include "AST/expressions.h"
#include "AST/statements.h"

//////////////////// CORRECT USE CASES ////////////////////

/*BOOST_AUTO_TEST_CASE( ParseFuncVoidTest )
{
    Parser parser(std::make_shared<SymbolTable>());

    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/fn_def_void.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);

    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE_NE(cNodes[0], nullptr);
}*/

BOOST_AUTO_TEST_CASE( ParseFuncZeroArgTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/fn_def_zero_arg.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::FUNCTION_DECL);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(cNodes[0].get());
    BOOST_REQUIRE(fDecl != nullptr);
    BOOST_REQUIRE_EQUAL(fDecl->GetFunctionName(), "MyFunc");
    
    const ParamVarDecls* params = fDecl->GetArguments();
    BOOST_REQUIRE(params != nullptr);
    BOOST_REQUIRE_EQUAL(params->GetParamNb(), 0);
    
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

/*BOOST_AUTO_TEST_CASE( ParseFuncOneArgTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/fn_def_one_arg.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);
}

BOOST_AUTO_TEST_CASE( ParseFuncMultiArgsTest )
{
    Parser parser(std::make_shared<SymbolTable>());

    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/fn_def_multi_args.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);
    BOOST_REQUIRE(cNodes[0] != nullptr);
}*/

//////////////////// ERROR USE CASES ////////////////////
