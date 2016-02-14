#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;
using namespace MachineEngine::ProcessorSpace;

InstructionSelector::InstructionSelector(const std::shared_ptr<SymbolTable>& symTab) : mNextRegister{ 0 }, mSymTable{ symTab }, mCurrentFunc{ nullptr }, mTreeToGraphMap{ }
{
    this->mPrologueFtr = [this]()
    {
        // Assign a register to the binary expression
        mNodeRegister[mCurrentNode] = ++mNextRegister;
    };
};


InstructionSelector::FunctionGraph InstructionSelector::Run(const std::unique_ptr<ASTNode>& root)
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
    {
        std::vector<VirtualInstruction> insts;
        // Generate a load instruction into the variable's register
        insts.emplace_back(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], mNodeRegister[initExpr]);

        CreateNewCurrentBlock(std::move(insts));
    }
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

    std::vector<VirtualInstruction> insts;
    // Generate the virtual instruction
    insts.emplace_back(opcode, mNodeRegister[bExpr->GetLHS()], mNodeRegister[bExpr->GetRHS()]);

    CreateNewCurrentBlock(std::move(insts));
}

void InstructionSelector::HandleBooleanExpr() 
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(mCurrentNode);
    assert(bExpr != nullptr);

    std::vector<VirtualInstruction> insts;
    // We simply load the boolean value into a register
    insts.emplace_back(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], bExpr->GetValue());

    CreateNewCurrentBlock(std::move(insts));
}

void InstructionSelector::HandleCallExpr() { }

void InstructionSelector::HandleIdentifierExpr() 
{
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(mCurrentNode);
    assert(iExpr != nullptr);

    std::vector<VirtualInstruction> insts;
    // We generate a load of the identifier into a new register.
    // This load is clearly redundant but it simplfies the instruction selection process.
    // TODO: Have a backend pass remove redundant load
    insts.emplace_back(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], mNodeRegister[iExpr]);

    CreateNewCurrentBlock(std::move(insts));
}

void InstructionSelector::HandleNumberExpr() 
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(mCurrentNode);
    assert(nExpr != nullptr);

    std::vector<VirtualInstruction> insts;
    // We simply load the number value into a register
    // TODO: There need to be a check in the semantic analyzer to ensure that the number value is reasonable
    insts.emplace_back(Instruction::LOAD_IMM, mNodeRegister[mCurrentNode], nExpr->GetValue());

    CreateNewCurrentBlock(std::move(insts));
}

// Statements
void InstructionSelector::HandleCompoundStmt()
{
    std::vector<VirtualInstruction> insts;
    // We add a block containing only a no-op instruction so that every AST node will
    // be matched with a basic block
    insts.emplace_back(Instruction::NO_OP);

    CreateNewCurrentBlock(std::move(insts));
}

void InstructionSelector::HandleIfStmt() 
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(mCurrentNode);
    assert(iStmt != nullptr);

    // Go back to the block associated with this AST node
    BasicBlock* condBlock = mTreeToGraphMap[mCurrentNode];

    // Link the current block with the condition block
    mCurrentBlock->InsertBranch(condBlock);

    // Switch over to the condition block
    mCurrentBlock = condBlock;

    // Generate a comparison instruction to prepare the flag register
    // before the branch
    mCurrentBlock->InsertInstruction({ Instruction::AND, mNodeRegister[iStmt->GetCondExpr()], 1 }); // TODO: more precision on the instruction type

    // Generate a branch instruction
    mCurrentBlock->InsertInstruction({Instruction::JUMP, 0}); // TODO: Correctly generate the branch

    // Create the exit block
    CreateNewCurrentBlock(std::vector<VirtualInstruction>{});

    // As per the instruction selection process the then statement should already be linked with
    // the condition expression. What we need to do now is link the then statement and the condition
    // expression (for when the condition evaluates to false) with the exit block
    condBlock->InsertBranch(mCurrentBlock);

    mTreeToGraphMap[iStmt->GetBody()]->InsertBranch(mCurrentBlock);
}

void InstructionSelector::HandlePrintStmt() { }

void InstructionSelector::HandleReturnStmt() { }

void InstructionSelector::HandleScanStmt() { }

void InstructionSelector::HandleWhileStmt() { }

void TosLang::BackEnd::InstructionSelector::CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts)
{
    BlockPtr newBlock = std::make_shared<BasicBlock>();

    for (auto& inst : insts)
        mCurrentBlock->InsertInstruction(inst);
    
    mCurrentCFG.InsertNode(newBlock);
    mCurrentBlock->InsertBranch(newBlock);
    mCurrentBlock = newBlock.get();
    mTreeToGraphMap[mCurrentNode] = mCurrentBlock;
}

