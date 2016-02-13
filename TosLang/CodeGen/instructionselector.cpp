#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;
using namespace MachineEngine::ProcessorSpace;

InstructionSelector::InstructionSelector(const std::shared_ptr<SymbolTable>& symTab) : mNextRegister{ 0 }, mSymTable{ symTab }, mCurrentFunc{ nullptr }
{
    this->mPrologueFtr = [this]()
    {
        // Assign a register to the binray expression
        mNodeRegister[mCurrentNode] = ++mNextRegister;
    };
};


InstructionSelector::FunctionGraph InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPreOrder(root);
    return mFunctionGraphs;
}

// Declarations
void InstructionSelector::HandleFunctionDecl() 
{
    // New function declaration so we need to build a new control flow graph
    ControlFlowGraph cfg;
}

void InstructionSelector::HandleVarDecl() 
{
    VarDecl* vDecl = dynamic_cast<VarDecl*>(mCurrentNode);
    assert(vDecl != nullptr);

    const Expr* initExpr = vDecl->GetInitExpr();

    // Instructions are only generated for variable with initialization expression
    if (initExpr != nullptr)
        // Generate a load instruction into the variable's register
        mCurrentBlock.InsertInstruction(VirtualInstruction(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], mNodeRegister[initExpr]));
}

// Expressions
void InstructionSelector::HandleBinaryExpr() 
{
    BinaryOpExpr* bExpr = dynamic_cast<BinaryOpExpr*>(mCurrentNode);
    assert(bExpr != nullptr);

    // Choose the correct opcode
    Instruction::InstructionOpCode opcode = Instruction::UNKNOWN;
    switch (bExpr->GetOperation())
    {
    case Common::Opcode::AND_BOOL:
    case Common::Opcode::AND_INT:
        opcode = Instruction::AND;
        break;
    case Common::Opcode::DIVIDE:
        opcode = Instruction::DIV;
        break;
    case Common::Opcode::GREATER_THAN:
        // TODO
        break;
    case Common::Opcode::LEFT_SHIFT:
        opcode = Instruction::SHIFT; // TODO: More precision
        break;
    case Common::Opcode::LESS_THAN:
        // TODO
        break;
    case Common::Opcode::MINUS:
        opcode = Instruction::SUB;
        break;
    case Common::Opcode::MODULO:
        opcode = Instruction::MOD;
        break;
    case Common::Opcode::MULT:
        opcode = Instruction::MUL;
        break;
    case Common::Opcode::NOT:
        opcode = Instruction::NOT;
        break;
    case Common::Opcode::OR_BOOL:
    case Common::Opcode::OR_INT:
        opcode = Instruction::OR;
        break;
    case Common::Opcode::PLUS:
        opcode = Instruction::ADD;
        break;
    case Common::Opcode::RIGHT_SHIFT:
        opcode = Instruction::SHIFT; // TODO: More precision
        break;
    default:
        break;
    }

    assert(opcode != Instruction::UNKNOWN);

    // Generate the virtual instruction
    mCurrentBlock.InsertInstruction(VirtualInstruction(opcode, mNodeRegister[bExpr->GetLHS()], mNodeRegister[bExpr->GetRHS()]));
}

void InstructionSelector::HandleBooleanExpr() 
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(mCurrentNode);
    assert(bExpr != nullptr);

    // We simply load the boolean value into a register
    mCurrentBlock.InsertInstruction(VirtualInstruction(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], bExpr->GetValue()));
}

void InstructionSelector::HandleCallExpr() { }

void InstructionSelector::HandleIdentifierExpr() 
{
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(mCurrentNode);
    assert(iExpr != nullptr);

    // We generate a load of the identifier into a new register.
    // This load is clearly redundant but it simplfies the instruction selection process.
    // TODO: Have a backend pass remove redundant load
    mCurrentBlock.InsertInstruction(VirtualInstruction(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], mNodeRegister[iExpr]));
}

void InstructionSelector::HandleNumberExpr() 
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(mCurrentNode);
    assert(nExpr != nullptr);

    // We simply load the number value into a register
    // TODO: There need to be a check in the semantic analyzer to ensure that the number value is reasonable
    mCurrentBlock.InsertInstruction(VirtualInstruction(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], nExpr->GetValue()));
}

// Statements
void InstructionSelector::HandleIfStmt() { }

void InstructionSelector::HandlePrintStmt() { }

void InstructionSelector::HandleReturnStmt() { }

void InstructionSelector::HandleScanStmt() { }

void InstructionSelector::HandleWhileStmt() { }

