#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

InstructionSelector::Program InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return mProgram;
}

// Declarations
void InstructionSelector::HandleFunctionDecl() { }

void InstructionSelector::HandleVarDecl() { }

// Expressions
void InstructionSelector::HandleBinaryExpr() { }

void InstructionSelector::HandleBooleanExpr() { }

void InstructionSelector::HandleCallExpr() { }

void InstructionSelector::HandleIdentifierExpr() { }

void InstructionSelector::HandleNumberExpr() { }

// Statements
void InstructionSelector::HandleIfStmt() { }

void InstructionSelector::HandlePrintStmt() { }

void InstructionSelector::HandleReturnStmt() { }

void InstructionSelector::HandleScanStmt() { }

void InstructionSelector::HandleWhileStmt() { }

