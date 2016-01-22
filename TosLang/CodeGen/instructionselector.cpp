#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

InstructionSelector::InstructionSelector(const std::shared_ptr<SymbolTable>& symTab) : mNextRegister{ 0 }, mSymTable{ symTab }, mCurrentFunc{ nullptr }
{
    this->mPrologueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            //mCurrentFunc = dynamic_cast<FunctionDecl*>(mCurrentNode);
            //assert(mCurrentFunc != nullptr);
        }
    };
};


InstructionSelector::Program InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return mProgram;
}

// Declarations
void InstructionSelector::HandleFunctionDecl() { }

void InstructionSelector::HandleVarDecl() 
{
    /*VarDecl* vDecl = dynamic_cast<VarDecl*>(mCurrentNode);
    const Expr* initExpr = vDecl->GetInitExpr();

    // Instructions are only generated for variable with initialization expression
    if (initExpr != nullptr)
    {

    }*/
}

// Expressions
void InstructionSelector::HandleBinaryExpr() { }

void InstructionSelector::HandleBooleanExpr() { }

void InstructionSelector::HandleCallExpr() { }

void InstructionSelector::HandleIdentifierExpr() 
{
    /*if (mNodeRegister.find(mCurrentNode) == mNodeRegister.end())
    {
        const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(mCurrentNode);
        mCurrentBlock.InsertInstruction({ Instruction::LOAD_IMM, ++mNextRegister, nExpr->GetValue() });
        mNodeRegister[mCurrentNode] = mNextRegister;
    }*/
}

void InstructionSelector::HandleNumberExpr() 
{
    /*mSymTable->IsGlobalVariable

    // A simple numeric constant will generate a single load from memory
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(mCurrentNode);
    mCurrentBlock.InsertInstruction({ Instruction::LOAD_IMM, ++mNextRegister, nExpr->GetValue() });
    mNodeRegister[mCurrentNode] = mNextRegister;*/
}

// Statements
void InstructionSelector::HandleIfStmt() { }

void InstructionSelector::HandlePrintStmt() { }

void InstructionSelector::HandleReturnStmt() { }

void InstructionSelector::HandleScanStmt() { }

void InstructionSelector::HandleWhileStmt() { }

