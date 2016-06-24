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

BOOST_AUTO_TEST_CASE( ParseArrayInitBoolTest )
{
    auto& cNodes = GetProgramAST("../sources/array/array_init_bool.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 2);
    
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::ARRAY_EXPR);
    const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(aExpr != nullptr);

    const ChildrenNodes& arrayElems = aExpr->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(arrayElems.size(), 2);
    BOOST_REQUIRE(arrayElems[0]->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    BOOST_REQUIRE(arrayElems[1]->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
}

BOOST_AUTO_TEST_CASE( ParseArrayInitIntTest )
{
    auto& cNodes = GetProgramAST("../sources/array/array_init_int.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 10);

    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::ARRAY_EXPR);
    const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(aExpr != nullptr);

    const ChildrenNodes& arrayElems = aExpr->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(arrayElems.size(), 10);

    for(size_t i = 0; i < arrayElems.size(); ++i)
        BOOST_REQUIRE(arrayElems[0]->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
}

BOOST_AUTO_TEST_CASE( ParseArrayInitIdentifierTest )
{
    auto& cNodes = GetProgramAST("../sources/array/array_init_identifier.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    BOOST_REQUIRE(cNodes[2]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(cNodes[2].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 2);

    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::ARRAY_EXPR);
    const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(aExpr != nullptr);

    const ChildrenNodes& arrayElems = aExpr->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(arrayElems.size(), 2);
    BOOST_REQUIRE(arrayElems[0]->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
    BOOST_REQUIRE(arrayElems[1]->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
}

BOOST_AUTO_TEST_CASE( ParseArrayInitStringTest )
{
    auto& cNodes = GetProgramAST("../sources/array/array_init_string.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = static_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyStringArrayVar");
    BOOST_REQUIRE_EQUAL(vDecl->GetVarSize(), 2);

    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::ARRAY_EXPR);
    const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(aExpr != nullptr);

    const ChildrenNodes& arrayElems = aExpr->GetChildrenNodes();
    BOOST_REQUIRE_EQUAL(arrayElems.size(), 2);
    BOOST_REQUIRE(arrayElems[0]->GetKind() == ASTNode::NodeKind::STRING_EXPR);
    BOOST_REQUIRE(arrayElems[1]->GetKind() == ASTNode::NodeKind::STRING_EXPR);
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseBadArrayExprTest )
{
    auto& cNodes = GetProgramAST("../sources/array/bad_array_expr.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    // We should only have error nodes
    BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }));

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 3);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Expected a ; at line 1, column 21");
    BOOST_REQUIRE_EQUAL(messages[1], "ERROR: Unexpected end of array expression at line 2, column 25");
    BOOST_REQUIRE_EQUAL(messages[2], "ERROR: Missing comma between two array elements at line 3, column 23");
}

BOOST_AUTO_TEST_SUITE_END()
