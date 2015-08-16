#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Parser
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "parser.h"

BOOST_AUTO_TEST_CASE( BadInitTest )
{
    Parser parser;
    BOOST_REQUIRE(parser.ParseProgram("BadFile.tos") == nullptr);
}

BOOST_AUTO_TEST_CASE( ParseVarDeclTest )
{
    Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/vardecl.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);
    
    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);

    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");

    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolVar");
}

BOOST_AUTO_TEST_CASE( ParseVarInitTest )
{
    Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/varinit.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    const ChildrenNodes& cNodes = pDecl->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(nExpr != nullptr);
    BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);

    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyTrueVar");
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    vDecl = dynamic_cast<const VarDecl*>(cNodes[2].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyFalseVar");
    bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr().get());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), false);
}
