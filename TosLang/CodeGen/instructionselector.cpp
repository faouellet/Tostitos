#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;
using namespace MachineEngine::ProcessorSpace;

std::unique_ptr<Module> InstructionSelector::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    // Reset the state of the instruction selector
    mNextRegister = 0;
    mNodeRegister.clear();
    mMod.reset(new Module{});
    mSymTable = symTab;

    HandleProgramDecl(root);
    return std::move(mMod);
}

// Declarations
void InstructionSelector::HandleProgramDecl(const std::unique_ptr<ASTNode>& root)
{
    for (auto& stmt : root->GetChildrenNodes())
    {
        if (stmt->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
            HandleFunctionDecl(stmt.get());
        else if (stmt->GetKind() == ASTNode::NodeKind::VAR_DECL)
            HandleVarDecl(stmt.get());
        else
            assert(false && "Unknown declaration in program");  // Shouldn't happen
    }
}

void InstructionSelector::HandleFunctionDecl(const ASTNode* decl)
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(decl);
    assert(fDecl != nullptr);

    // Resetting the memory slot index for local variables
    mLocalMemSlot = 0;

    // New function declaration so we need to build a new control flow graph
    CFGPtr pCFG = std::make_shared<ControlFlowGraph>();
    mCurrentFunc = pCFG.get();
    mCurrentBlock = mCurrentFunc->CreateNewBlock().get();

    mMod->InsertFunction(fDecl->GetFunctionName(), pCFG);

    // TODO: Insert instructions to fetch the arguements

    HandleCompoundStmt(fDecl->GetBody());

    // Removing ties to the function
    mCurrentBlock = nullptr;
    mCurrentFunc = nullptr;
}

void InstructionSelector::HandleVarDecl(const ASTNode* decl) 
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(decl);
    assert(vDecl != nullptr);

    mNodeRegister[decl] = mNextRegister++;
    const bool isGlobalVar = mSymTable->IsGlobalVariable(vDecl->GetName());

    // If we have a local variable (function parameters are also local variables), we generate instructions for the stack frame
    if (vDecl->IsFunctionParameter() || !isGlobalVar)
    {
        // Generate an allocation in the stack frame
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::ALLOCA }
                                         .AddMemSlotOperand(mLocalMemSlot++));

        // Store the variable in the space allocated
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::STORE }
                                         .AddRegOperand(mNodeRegister[decl])
                                         .AddMemSlotOperand(mLocalMemSlot));
    
        // For a function parameter nothing else needs to be done
        if (vDecl->IsFunctionParameter())
            return;
    }

    const Expr* initExpr = vDecl->GetInitExpr();

    VirtualInstruction vInst;

    // Generate an instruction to load the initialization expression into the variable
    if (initExpr != nullptr)
    {
        HandleExpr(initExpr);

        // Generate a load instruction into the variable's register
        vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD }
                                    .AddRegOperand(mNodeRegister[decl])
                                    .AddRegOperand(mNodeRegister[initExpr]);
    }
    // If there's no initialization expression, the variable will be default initialized
    else
    {
        // TODO: Handle StringExpr

        vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD }
                                    .AddRegOperand(mNodeRegister[decl])
                                    .AddImmOperand(0);
    }
    
    if (isGlobalVar)
        mMod->InsertGlobalVar(vInst);
    else
        mCurrentBlock->InsertInstruction(vInst);
}

// Expressions
void InstructionSelector::HandleExpr(const Expr* expr)
{
    mNodeRegister[expr] = mNextRegister++;

    switch (expr->GetKind())
    {
    case ASTNode::NodeKind::BINARY_EXPR:
        HandleBinaryExpr(expr);
        break;
    case ASTNode::NodeKind::BOOLEAN_EXPR:
    {
        const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(expr);
        assert(bExpr != nullptr);

        auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD_IMM }
                     .AddRegOperand(mNodeRegister[expr])
                     .AddImmOperand(bExpr->GetValue());

        if (mCurrentBlock != nullptr)
            mCurrentBlock->InsertInstruction(vInst);
        else
            mMod->InsertGlobalVar(vInst);
    }
        break;
    case ASTNode::NodeKind::CALL_EXPR:
        HandleCallExpr(expr);
        break;
    case ASTNode::NodeKind::IDENTIFIER_EXPR:
    {
        const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(expr);
        assert(iExpr != nullptr);

        auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD_IMM }
                     .AddRegOperand(mNodeRegister[expr])
                     .AddRegOperand(mNodeRegister[iExpr]);

        if (mCurrentBlock != nullptr)
            mCurrentBlock->InsertInstruction(vInst);
        else
            mMod->InsertGlobalVar(vInst);
    }
        break;
    case ASTNode::NodeKind::NUMBER_EXPR:
    {
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(expr);
        assert(nExpr != nullptr);

        auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD_IMM }
                     .AddRegOperand(mNodeRegister[expr])
                     .AddImmOperand(nExpr->GetValue());

        if (mCurrentBlock != nullptr)
            mCurrentBlock->InsertInstruction(vInst);
        else
            mMod->InsertGlobalVar(vInst);
    }
        break;
    case ASTNode::NodeKind::STRING_EXPR:
    {
        const StringExpr* sExpr = dynamic_cast<const StringExpr*>(expr);
        assert(sExpr != nullptr);

        // We add the string literal to the module
        unsigned memSlot = mMod->InsertArrayVariable(sExpr->GetName(), sExpr->GetName()); // Name and value are the same for simplicity

        // We generate a load of the string literal address
        auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD_IMM }
                     .AddRegOperand(mNodeRegister[expr])
                     .AddMemSlotOperand(memSlot);

        if (mCurrentBlock != nullptr)
            mCurrentBlock->InsertInstruction(vInst);
        else
            mMod->InsertGlobalVar(vInst);
    }
        break;
    default:
        assert(false && "Unknown expression node");
        break;
    }
}

void InstructionSelector::HandleBinaryExpr(const ASTNode* expr)
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(expr);
    assert(bExpr != nullptr);

    // TODO: Assignment

    // Choose the correct opcode
    VirtualInstruction::Opcode opcode = VirtualInstruction::Opcode::UNKNOWN;
    switch (bExpr->GetOperation())
    {
    case Common::Opcode::AND_BOOL:
    case Common::Opcode::AND_INT:
        opcode = VirtualInstruction::Opcode::AND;
        break;
    case Common::Opcode::DIVIDE:
        opcode = VirtualInstruction::Opcode::DIV;
        break;
    case Common::Opcode::GREATER_THAN:
        opcode = VirtualInstruction::Opcode::GT;
        break;
    case Common::Opcode::LEFT_SHIFT:
        opcode = VirtualInstruction::Opcode::LSHIFT;
        break;
    case Common::Opcode::LESS_THAN:
        opcode = VirtualInstruction::Opcode::LT;
        break;
    case Common::Opcode::MINUS:
        opcode = VirtualInstruction::Opcode::SUB;
        break;
    case Common::Opcode::MODULO:
        opcode = VirtualInstruction::Opcode::MOD;
        break;
    case Common::Opcode::MULT:
        opcode = VirtualInstruction::Opcode::MUL;
        break;
    case Common::Opcode::NOT:
        opcode = VirtualInstruction::Opcode::NOT;
        break;
    case Common::Opcode::OR_BOOL:
    case Common::Opcode::OR_INT:
        opcode = VirtualInstruction::Opcode::OR;
        break;
    case Common::Opcode::PLUS:
        opcode = VirtualInstruction::Opcode::ADD;
        break;
    case Common::Opcode::RIGHT_SHIFT:
        opcode = VirtualInstruction::Opcode::RSHIFT;
        break;
    default:
        break;
    }

    assert(opcode != VirtualInstruction::Opcode::UNKNOWN);

    // Handle the expression's operands
    HandleExpr(bExpr->GetLHS());
    HandleExpr(bExpr->GetRHS());

    // Generate the virtual instruction
    if (mCurrentBlock != nullptr)
    {
        mCurrentBlock->InsertInstruction(VirtualInstruction{ opcode }
                                         .AddRegOperand(mNodeRegister[bExpr->GetLHS()])
                                         .AddRegOperand(mNodeRegister[bExpr->GetRHS()]));
    }
    else
    {
        mMod->InsertGlobalVar(VirtualInstruction{ opcode }
                              .AddRegOperand(mNodeRegister[bExpr->GetLHS()])
                              .AddRegOperand(mNodeRegister[bExpr->GetRHS()]));
    }
    
}

void InstructionSelector::HandleCallExpr(const ASTNode* expr) 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(expr);
    assert(cExpr != nullptr);

    // TODO: what to do with the function arguments?

    // We generate a call instruction. This will take care of the stack pointer.
    mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::CALL }
                                     // TODO: Quite a mouthful
                                     .AddTargetOperand(mMod->GetFunction(cExpr->GetCalleeName())->GetEntryBlock().get()));
}

// Statements
BlockPtr InstructionSelector::HandleCompoundStmt(const CompoundStmt* cStmt)
{
    CreateNewCurrentBlock();
    BlockPtr entryBlock{ mCurrentBlock };

    for (auto& stmt : cStmt->GetStatements())
    {
        switch (stmt->GetKind())
        {
        case ASTNode::NodeKind::BINARY_EXPR:
            HandleBinaryExpr(stmt.get());
            break;
        case ASTNode::NodeKind::CALL_EXPR:
            HandleCallExpr(stmt.get());
            break;
        case ASTNode::NodeKind::IF_STMT:
            HandleIfStmt(stmt.get());
            break;
        case ASTNode::NodeKind::PRINT_STMT:
            HandlePrintStmt(stmt.get());
            break;
        case ASTNode::NodeKind::RETURN_STMT:
            HandleReturnStmt(stmt.get());
            break;
        case ASTNode::NodeKind::SCAN_STMT:
            HandleScanStmt(stmt.get());
            break;
        case ASTNode::NodeKind::VAR_DECL:
            HandleVarDecl(stmt.get());
            break;
        case ASTNode::NodeKind::WHILE_STMT:
            HandleWhileStmt(stmt.get());
            break;
        default:
            break;
        }
    }

    return entryBlock;
}

void InstructionSelector::HandleIfStmt(const ASTNode* stmt)
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(stmt);
    assert(iStmt != nullptr);

    // Generating instructions for the condition expression
    HandleExpr(iStmt->GetCondExpr());

    // Keeping a pointer to the condition block for later
    BasicBlock* condBlock = mCurrentBlock;
    
    // Generating instructions for the if body
    BlockPtr thenBeginBlock = HandleCompoundStmt(iStmt->GetBody());
    
    // Keep a pointer to the current block which correspond to the end of the if body
    BasicBlock* thenEndBlock = mCurrentBlock;

    // Creating the exit block
    CreateNewCurrentBlock();
    
    // Generating a branch instruction that goes from the condition block to the then (body) begin block
    condBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                 .AddRegOperand(mNodeRegister[iStmt->GetCondExpr()])
                                 .AddTargetOperand(thenBeginBlock.get())
                                 .AddTargetOperand(mCurrentBlock));

    // Generating a branch instruction from the then (body) end block to the exit block
    thenEndBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                    .AddTargetOperand(mCurrentBlock));
}

void InstructionSelector::HandlePrintStmt(const ASTNode* stmt)
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(stmt);
    assert(pStmt != nullptr);

    // TODO
}

void InstructionSelector::HandleReturnStmt(const ASTNode* stmt) 
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmt);
    assert(rStmt != nullptr);

    if (rStmt->GetReturnExpr())
    {
        // TODO: What to do with the returned value (when there is one)?
    }

    // We generate a return opcode. This will take care of dealing with the stack pointer.
    mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::RET });
}

void InstructionSelector::HandleScanStmt(const ASTNode* stmt)
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(stmt);
    assert(sStmt != nullptr);

    // TODO
}

void InstructionSelector::HandleWhileStmt(const ASTNode* stmt)
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(stmt);
    assert(wStmt != nullptr);

    // Creating the loop header (condition block)
    CreateNewCurrentBlock();

    // Generating instructions for the condition expression
    HandleExpr(wStmt->GetCondExpr());

    // Keeping a pointer to the header block for later
    BasicBlock* headerBlock = mCurrentBlock;

    // Generating instructions for the while body
    BlockPtr bodyBeginBlock = HandleCompoundStmt(wStmt->GetBody());

    // Keep a pointer to the current block which correspond to the end of the if body
    BasicBlock* bodyEndBlock = mCurrentBlock;

    // Creating the exit block
    CreateNewCurrentBlock();

    // Generating a branch instruction that goes from the header block to either the body begin block or the exit block
    headerBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                   .AddRegOperand(mNodeRegister[wStmt->GetCondExpr()])
                                   .AddTargetOperand(bodyBeginBlock.get())
                                   .AddTargetOperand(mCurrentBlock));

    // Generating a branch instruction from the body end block to the header block
    bodyEndBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                    .AddTargetOperand(headerBlock));
}

void TosLang::BackEnd::InstructionSelector::CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts)
{
    BlockPtr newBlock = mCurrentFunc->CreateNewBlock(std::move(insts));
    mCurrentBlock->InsertBranch(newBlock);
    mCurrentBlock = newBlock.get();
}

