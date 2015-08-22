#include "instructionselector.h"

std::vector<Instruction> InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return mProgram;
}

// Declarations

void InstructionSelector::HandleProgramDecl()
{

}

void InstructionSelector::HandleVarDecl()
{

}

// Expressions

void InstructionSelector::HandleBooleanExpr()
{

}

void InstructionSelector::HandleNumberExpr()
{

}
