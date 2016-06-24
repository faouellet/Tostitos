#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserArrayTests
#endif
#endif

#include "toslang_parser_fixture.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseArrayDeclTest )
{
    auto& cNodes = GetProgramAST("../sources/array/array_decl.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 10);

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = static_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 10);

    BOOST_REQUIRE(cNodes[2]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = static_cast<const VarDecl*>(cNodes[2].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyStringArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 10);
}


//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_SUITE_END()
