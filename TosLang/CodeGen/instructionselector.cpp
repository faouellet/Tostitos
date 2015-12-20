#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;
using namespace MachineEngine::ProcessorSpace;

std::vector<Instruction> InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return GenerateProgram();
}

// Declarations

void InstructionSelector::HandleVarDecl(){ }

// Expressions

void HandleBinaryExpr() { }

void HandleBooleanExpr() { }

void HandleCallExpr() { }

void HandleIdentifierExpr() { }

void HandleNumberExpr() { }

// Code Generation

std::vector<Instruction> InstructionSelector::GenerateProgram() 
{
    std::vector<Instruction> program;
    
    return program;
}

Instruction InstructionSelector::GenerateInstruction(const ASTNode* node, Instruction::InstructionType iType)
{
    Instruction inst(iType);
    // TODO: Rethink about the basic register allocation done here. Instead of using a real Chip16 instruction,
    //       could a virtual instruction be used instead so as not to go beyond 256 allocated registers?
    //inst.SetFirstOperand(mNextRegister++);

    if (inst.UseImmediateValue())
    {
        if (node->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR)
        {
            const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
            assert(bExpr != nullptr);
            inst.SetImmediateValue(bExpr->GetValue());

        }
        else if (node->GetKind() == ASTNode::NodeKind::NUMBER_EXPR)
        {
            const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(node);
            assert(nExpr != nullptr);
            // TODO: A decision should be made about the maximum size of an integer (16 or 32 bits?)
            //inst.SetImmediateValue(nExpr->GetValue());
        }
    }
    else
    {
        /*inst.SetSecondOperand(mNextRegister++);
        if (!inst.IsInplace())
            inst.SetThirdOperand(mNextRegister++);*/
    }

    return inst;
}
