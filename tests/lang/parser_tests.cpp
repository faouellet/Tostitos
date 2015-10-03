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
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/var_decl.tos");
    BOOST_REQUIRE(rootNode != nullptr);
    
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);
    
    auto& cNodes = pDecl->GetProgramStmts();
    BOOST_REQUIRE_EQUAL(cNodes.size(), 3);
	BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

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
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/var_init_bool.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
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
	Parser parser(std::make_shared<SymbolTable>());
	std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/var_init_int.tos");
	BOOST_REQUIRE(rootNode != nullptr);

	BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
	const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
	BOOST_REQUIRE(pDecl != nullptr);

	auto& cNodes = pDecl->GetProgramStmts();
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
	Parser parser(std::make_shared<SymbolTable>());
	std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/var_init_identifier.tos");
	BOOST_REQUIRE(rootNode != nullptr);

	BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
	const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
	BOOST_REQUIRE(pDecl != nullptr);

	auto& cNodes = pDecl->GetProgramStmts();
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
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/binary_op_int.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    BOOST_REQUIRE(rootNode->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
    const ProgramDecl* pDecl = dynamic_cast<const ProgramDecl*>(rootNode.get());
    BOOST_REQUIRE(pDecl != nullptr);

    auto& cNodes = pDecl->GetProgramStmts();
    const size_t childExpectedSize = 9;
    BOOST_REQUIRE_EQUAL(cNodes.size(), childExpectedSize);

    Opcode operations[] =
    { 
        Opcode::PLUS,
        Opcode::MINUS,
        Opcode::MULT,
        Opcode::DIVIDE,
        Opcode::MODULO,
        Opcode::AND_INT,
        Opcode::OR_INT,
        Opcode::RIGHT_SHIFT,
        Opcode::LEFT_SHIFT,
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
