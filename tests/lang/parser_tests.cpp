#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Parser
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "Parse/parser.h"
#include "AST/declarations.h"
#include "AST/expressions.h"

using namespace TosLang::FrontEnd;

BOOST_AUTO_TEST_CASE( ParseVarDeclTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/vardecl.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);
    
    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
	BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

    BOOST_REQUIRE_EQUAL(cNodes[0]->GetKind(), ASTNode::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");

    BOOST_REQUIRE_EQUAL(cNodes[0]->GetKind(), ASTNode::VAR_DECL);
    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolVar");
}

BOOST_AUTO_TEST_CASE( ParseVarInitTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/varinit.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    BOOST_REQUIRE_EQUAL(rootNode->GetKind(), ASTNode::PROGRAM_DECL);
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    BOOST_REQUIRE_EQUAL(cNodes[0]->GetKind(), ASTNode::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetInitExpr()->GetKind(), ASTNode::NUMBER_EXPR);
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);

    BOOST_REQUIRE_EQUAL(cNodes[0]->GetKind(), ASTNode::VAR_DECL);
    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyTrueVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetInitExpr()->GetKind(), ASTNode::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    BOOST_REQUIRE_EQUAL(cNodes[0]->GetKind(), ASTNode::VAR_DECL);
    vDecl = dynamic_cast<const VarDecl*>(cNodes[2].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyFalseVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetInitExpr()->GetKind(), ASTNode::BOOLEAN_EXPR);
    bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), false);
}
