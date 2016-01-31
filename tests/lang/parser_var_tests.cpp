#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ParserVarTests
#endif
#endif

#include "toslang_fixture.h"

#include "Common/opcodes.h"

BOOST_FIXTURE_TEST_SUITE( FrontEndTestSuite, TosLangFixture )

//////////////////// CORRECT USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParseVarDeclTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/var_decl.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyBoolVar");

	BOOST_REQUIRE(cNodes[2]->GetKind() == ASTNode::NodeKind::VAR_DECL);
	vDecl = dynamic_cast<const VarDecl*>(cNodes[2].get());
	BOOST_REQUIRE(vDecl != nullptr);
	BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyStringVar");
}

BOOST_AUTO_TEST_CASE( ParseVarInitBoolTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/var_init_bool.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 2);
	
    BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
	const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyTrueVar");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), true);

    BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
    vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
    BOOST_REQUIRE(vDecl != nullptr);
    BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyFalseVar");
    BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR);
    bExpr = dynamic_cast<const BooleanExpr*>(vDecl->GetInitExpr());
    BOOST_REQUIRE(bExpr != nullptr);
    BOOST_REQUIRE_EQUAL(bExpr->GetValue(), false);
}

BOOST_AUTO_TEST_CASE( ParseVarInitIntTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/var_init_int.tos");
	BOOST_REQUIRE_EQUAL(cNodes.size(), 1);

	BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
	const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
	BOOST_REQUIRE(vDecl != nullptr);
	BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");
	BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
	const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr());
	BOOST_REQUIRE(nExpr != nullptr);
	BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);
}

BOOST_AUTO_TEST_CASE( ParseVarInitIdentifierTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/var_init_identifier.tos");
	BOOST_REQUIRE_EQUAL(cNodes.size(), 2);

	BOOST_REQUIRE(cNodes[0]->GetKind() == ASTNode::NodeKind::VAR_DECL);
	const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[0].get());
	BOOST_REQUIRE(vDecl != nullptr);
	BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar");
	BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::NUMBER_EXPR);
	const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(vDecl->GetInitExpr());
	BOOST_REQUIRE(nExpr != nullptr);
	BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 42);

	BOOST_REQUIRE(cNodes[1]->GetKind() == ASTNode::NodeKind::VAR_DECL);
	vDecl = dynamic_cast<const VarDecl*>(cNodes[1].get());
	BOOST_REQUIRE(vDecl != nullptr);
	BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "MyIntVar2");
	BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
	const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(vDecl->GetInitExpr());
	BOOST_REQUIRE(iExpr != nullptr);
	BOOST_REQUIRE_EQUAL(iExpr->GetName(), "MyIntVar");
}

BOOST_AUTO_TEST_CASE( ParseVarInitIntBinOpTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/binary_op_int.tos");
    const size_t childExpectedSize = 9;
    BOOST_REQUIRE_EQUAL(cNodes.size(), childExpectedSize);

    TosLang::Common::Opcode operations[] =
    { 
        TosLang::Common::Opcode::PLUS,
        TosLang::Common::Opcode::MINUS,
        TosLang::Common::Opcode::MULT,
        TosLang::Common::Opcode::DIVIDE,
        TosLang::Common::Opcode::MODULO,
        TosLang::Common::Opcode::AND_INT,
        TosLang::Common::Opcode::OR_INT,
        TosLang::Common::Opcode::RIGHT_SHIFT,
        TosLang::Common::Opcode::LEFT_SHIFT,
    };

    for (size_t i = 0; i < childExpectedSize; i++)
    {
        BOOST_REQUIRE(cNodes[i]->GetKind() == ASTNode::NodeKind::VAR_DECL);
        const VarDecl* vDecl = dynamic_cast<const VarDecl*>(cNodes[i].get());
        BOOST_REQUIRE(vDecl != nullptr);
        BOOST_REQUIRE_EQUAL(vDecl->GetVarName(), "V" + std::to_string(i + 1));
        
        BOOST_REQUIRE(vDecl->GetInitExpr()->GetKind() == ASTNode::NodeKind::BINARY_EXPR);
        const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(vDecl->GetInitExpr());
        BOOST_REQUIRE(bExpr != nullptr);
        BOOST_REQUIRE(bExpr->GetOperation() == operations[i]);
        
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(bExpr->GetLHS());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 1);

        nExpr = dynamic_cast<const NumberExpr*>(bExpr->GetRHS());
        BOOST_REQUIRE(nExpr != nullptr);
        BOOST_REQUIRE_EQUAL(nExpr->GetValue(), 1);
    }
}

//////////////////// ERROR USE CASES ////////////////////

BOOST_AUTO_TEST_CASE( ParserBadInitTest )
{
    Parser parser;
    BOOST_REQUIRE(parser.ParseProgram("../inputs/var/var_decl.cpp") == nullptr);
    BOOST_REQUIRE(parser.ParseProgram("BadFile.tos") == nullptr);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "FILE ERROR: Wrong file type");
    BOOST_REQUIRE_EQUAL(messages[1], "FILE ERROR: Problem opening the specified file");
}

BOOST_AUTO_TEST_CASE( ParseBadVarDeclTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/bad_var_decl.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 10);

    // Check that there is all but one non error node
    // TODO: This should be re-enabled once the type checker is workinh again
    //BOOST_REQUIRE_EQUAL(std::count_if(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node){ return node->GetKind() != ASTNode::NodeKind::ERROR; }), 1);
    
    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 8);
    BOOST_REQUIRE_EQUAL(messages[0], "VAR ERROR: The var keyword should be followed by an identifier at line 1, column 3");
    BOOST_REQUIRE_EQUAL(messages[1], "VAR ERROR: The var keyword should be followed by an identifier at line 2, column 7");
    BOOST_REQUIRE_EQUAL(messages[2], "VAR ERROR: Missing : between a variable and its type at line 3, column 12");
    BOOST_REQUIRE_EQUAL(messages[3], "VAR ERROR: Missing : between a variable and its type at line 4, column 8");
    BOOST_REQUIRE_EQUAL(messages[4], "VAR ERROR: Missing type from variable declaration at line 5, column 8");
    BOOST_REQUIRE_EQUAL(messages[5], "ERROR: Expected a ; at line 7, column 3");
    BOOST_REQUIRE_EQUAL(messages[6], "VAR ERROR: The var keyword should be followed by an identifier at line 7, column 3");
    BOOST_REQUIRE_EQUAL(messages[7], "VAR ERROR: Trying to create a variable with void type at line 10, column 17");
    
    // TODO: This should be re-enabled once the type checker is workinh again
    // BOOST_REQUIRE_EQUAL(messages[7], "VAR ERROR: Trying to redefine an already defined variable at line 9, column 14");
}

BOOST_AUTO_TEST_CASE( ParseBadVarInitBinOpTest )
{
    auto& cNodes = GetProgramAST("../inputs/var/bad_binary_op.tos");
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);

    // Check that we have only error nodes.
    BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node->GetKind() == ASTNode::NodeKind::ERROR; }));

    // Check if the correct error messages got printed
    std::vector<std::string> messages{ GetErrorMessages() };
    BOOST_REQUIRE_EQUAL(messages.size(), 3);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Not an acceptable use of a binary operation at line 1, column 14");
    BOOST_REQUIRE_EQUAL(messages[1], "ERROR: Missing right hand side in binary expression at line 2, column 15");
    BOOST_REQUIRE_EQUAL(messages[2], "ERROR: Not an acceptable binary operation at line 3, column 18");
}

BOOST_AUTO_TEST_SUITE_END()
