#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;
using namespace MachineEngine::ProcessorSpace;

InstructionSelector::InstructionSelector() : 
    mNextRegister{ 0 }, mCurrentFunc{ nullptr }, mTreeToGraphMap{ }
{
    mMod.reset(new Module{});

    this->mPrologueFtr = [this]()
    {
        // Assign a register to AST node
        mNodeRegister[mCurrentNode] = mNextRegister++;

        // If a node is a statement node, it will generate a new basic block
        switch (mCurrentNode->GetKind())
        {
        case ASTNode::NodeKind::COMPOUND_STMT:
        case ASTNode::NodeKind::IF_STMT:
        case ASTNode::NodeKind::WHILE_STMT:
            CreateNewCurrentBlock(std::vector<VirtualInstruction>{});
        default:
            break;
        }
    };
};


std::unique_ptr<Module> InstructionSelector::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    // Reset the module to be produced
    mMod.reset(new Module{});

    mSymTable = symTab;

    this->VisitPreOrder(root);
    return std::move(mMod);
}

// Declarations
void InstructionSelector::HandleFunctionDecl() 
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(mCurrentNode);
    assert(fDecl != nullptr);

    // New function declaration so we need to build a new control flow graph
    CFGPtr pCFG = std::make_shared<ControlFlowGraph>();
    mCurrentCFG = pCFG.get();
    mCurrentBlock = mCurrentCFG->CreateNewBlock().get();

    mMod->InsertFunction(fDecl->GetFunctionName(), pCFG);

    // TODO: Insert instructions to fetch the arguements
}

void InstructionSelector::HandleVarDecl() 
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(mCurrentNode);
    assert(vDecl != nullptr);

    const Expr* initExpr = vDecl->GetInitExpr();

    // Instructions are only generated for variable with initialization expression
    if (initExpr != nullptr)
    {
        // Generate a load instruction into the variable's register
        VirtualInstruction vInst = VirtualInstruction{ Instruction::LOAD_IMM }
                                   .AddRegOperand(mNodeRegister[mCurrentNode])
                                   .AddRegOperand(mNodeRegister[initExpr]);

        if (mSymTable->IsGlobalVariable(vDecl->GetName()))
        {
            mMod->InsertGlobalVar(vInst);
        }
        else
        {
            mCurrentBlock->InsertInstruction(vInst);
        }
    }
}

// Expressions
void InstructionSelector::HandleBinaryExpr() 
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(mCurrentNode);
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
    mCurrentBlock->InsertInstruction(VirtualInstruction{ opcode }
                                     .AddRegOperand(mNodeRegister[bExpr->GetLHS()])
                                     .AddRegOperand(mNodeRegister[bExpr->GetRHS()]));
}

void InstructionSelector::HandleBooleanExpr() 
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(mCurrentNode);
    assert(bExpr != nullptr);

    // We simply load the boolean value into a register
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::LOAD_IMM }
                                     .AddRegOperand(mNodeRegister[mCurrentNode])
                                     .AddImmOperand(bExpr->GetValue()));
}

void InstructionSelector::HandleCallExpr() 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(mCurrentNode);
    assert(cExpr != nullptr);

    // TODO: what to do with the function arguments?

    // We generate a call instruction. This will take care of the stack pointer.
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::CALL }
                                     // TODO: Quite a mouthful
                                     .AddTargetOperand(mMod->GetFunction(cExpr->GetCalleeName())->GetEntryBlock().get()));
}

void InstructionSelector::HandleIdentifierExpr() 
{
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(mCurrentNode);
    assert(iExpr != nullptr);

    // We generate a load of the identifier into a new register.
    // This load is clearly redundant but it simplfies the instruction selection process.
    // TODO: Have a backend pass remove redundant load
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::LOAD_IMM }
                                     .AddRegOperand(mNodeRegister[mCurrentNode])
                                     .AddRegOperand(mNodeRegister[iExpr]));
}

void InstructionSelector::HandleNumberExpr() 
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(mCurrentNode);
    assert(nExpr != nullptr);

    // We simply load the number value into a register
    // TODO: There need to be a check in the semantic analyzer to ensure that the number value is reasonable
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::LOAD_IMM }
                                     .AddRegOperand(mNodeRegister[mCurrentNode])
                                     .AddImmOperand(nExpr->GetValue()));
}

void InstructionSelector::HandleStringExpr()
{
    const StringExpr* sExpr = dynamic_cast<const StringExpr*>(mCurrentNode);
    assert(sExpr != nullptr);

    // We add the string literal to the module
    unsigned memSlot = mMod->InsertArrayVariable(sExpr->GetName(), sExpr->GetName()); // Name and value are the same for simplicity

    // We generate a load of the string literal address
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::LOAD_IMM }
                                     .AddRegOperand(mNodeRegister[mCurrentNode])
                                     .AddMemSlotOperand(memSlot));
}

// Statements
void InstructionSelector::HandleIfStmt()
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(mCurrentNode);
    assert(iStmt != nullptr);

    // Go back to the block associated with this AST node
    BasicBlock* ifBlock = mTreeToGraphMap[mCurrentNode];

    // Keep a pointer to the current block which correspond to the end of the then statement
    BasicBlock* thenEndBlock = mCurrentBlock;

    // Create the exit block
    CreateNewCurrentBlock(std::vector<VirtualInstruction>{});
    BasicBlock* endBlock = mCurrentBlock;

    // Link the then statement end with the exit block
    thenEndBlock->InsertBranch(mCurrentBlock);

    // Done with the then statement, switch over to the if block
    mCurrentBlock = ifBlock;

    // Generate a comparison instruction to prepare the flag register
    // before the branch
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::TST }
                                     .AddRegOperand(mNodeRegister[iStmt->GetCondExpr()])
                                     .AddImmOperand(1));

    // Generate a branch instruction to the then block
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::JUMP }
                                     .AddTargetOperand(ifBlock->bb_begin()->get()));
    // Generate a branch instruction to the exit block
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::JUMP }
                                     .AddTargetOperand(endBlock));

    // As per the instruction selection process the then statement should already be linked with
    // the condition expression. What we need to do now is link the condition
    // expression (for when the condition evaluates to false) with the exit block
    ifBlock->InsertBranch(endBlock);
}

void InstructionSelector::HandlePrintStmt() { }

void InstructionSelector::HandleReturnStmt() 
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(mCurrentNode);
    assert(rStmt != nullptr);

    if (rStmt->GetReturnExpr())
    {
        // TODO: What to do with the returned value (when there is one)?
    }

    // We generate a return opcode. This will take care of dealing with the stack pointer.
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::RET });
}

void InstructionSelector::HandleScanStmt() { }

void InstructionSelector::HandleWhileStmt() 
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(mCurrentNode);
    assert(wStmt != nullptr);

    // Go back to the block associated with this AST node
    BasicBlock* whileBlock = mTreeToGraphMap[mCurrentNode];
    
    // Keep a pointer to the current block which correspond to the end of the then statement
    BasicBlock* thenEndBlock = mCurrentBlock;

    // Insert a jump instruction that points back to the condition block
    thenEndBlock->InsertInstruction(VirtualInstruction{Instruction::JUMP}
                                    .AddTargetOperand(whileBlock));

    // Link the end of the then statement with the condition block
    thenEndBlock->InsertBranch(whileBlock);

    // Done with the then statement, switch over to the condition block
    mCurrentBlock = whileBlock;

    // Generate a comparison instruction to prepare the flag register before the branch
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::TST }
                                     .AddRegOperand(mNodeRegister[wStmt->GetCondExpr()])
                                     .AddImmOperand(1));

    // Generate a branch instruction to the then block
    mCurrentBlock->InsertInstruction(VirtualInstruction{ Instruction::JUMP }
                                     .AddTargetOperand(whileBlock->bb_begin()->get())); 
    
    // Generate a branch instruction to the exit block
    CreateNewCurrentBlock(std::vector<VirtualInstruction>{});
}

void TosLang::BackEnd::InstructionSelector::CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts)
{
    BlockPtr newBlock = mCurrentCFG->CreateNewBlock(std::move(insts));
    mCurrentBlock->InsertBranch(newBlock);
    mCurrentBlock = newBlock.get();
    mTreeToGraphMap[mCurrentNode] = mCurrentBlock;
}

