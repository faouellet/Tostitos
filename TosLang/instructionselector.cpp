#include "instructionselector.h"

#include "declarations.h"
#include "expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

std::vector<Instruction> InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return GenerateProgram();
}

// Declarations

void InstructionSelector::HandleVarDecl()
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
    assert(vDecl != nullptr);

    // An instruction that is not written to will not generate any assembly
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr().get());
    if (initExpr != nullptr)
    {
        // TODO: Use ResultType of child nodes
        std::vector<Rule> rules{ mRuleTable.MatchPattern(std::make_pair(OperatorType::OP_ASSIGN, 
            std::vector<OperandType>{ OperandType::REGISTER, OperandType::IMMEDIATE })) };
        assert(rules.size() == 1);
 
        mNodeLabels[this->mCurrentNode] = rules[0].mID;
        mNodesVisited.push(this->mCurrentNode);
    }
}

// Expressions

// Code Generation

std::vector<Instruction> InstructionSelector::GenerateProgram() 
{
    std::vector<Instruction> program;

    while (!mNodesVisited.empty())
    {
        const ASTNode* node = mNodesVisited.top();
        program.push_back(GenerateInstruction(node, mRuleTable.GetRule(mNodeLabels[node]).mInstType));
        mNodesVisited.pop();
    }

    return program;
}

Instruction InstructionSelector::GenerateInstruction(const ASTNode* node, Instruction::InstructionType iType)
{
    Instruction inst(iType);
    inst.SetFirstOperand(mNextRegister++);

    if (inst.UseImmediateValue())
    {
        if (node->GetKind() == ASTNode::BOOLEAN_EXPR)
        {
            const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
            assert(bExpr != nullptr);
            inst.SetImmediateValue(bExpr->GetValue());

        }
        else if (node->GetKind() == ASTNode::NUMBER_EXPR)
        {
            const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(node);
            assert(nExpr != nullptr);
            inst.SetImmediateValue(nExpr->GetValue());
        }
    }
    else
    {
        inst.SetSecondOperand(mNextRegister++);
        if (!inst.IsInplace())
            inst.SetThirdOperand(mNextRegister++);
    }

    return inst;
}
