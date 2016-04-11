#include "instructionselector.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

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
    mCurrentFunc = fDecl;
    
    // New function declaration so we need to build a new control flow graph
    CFGPtr pCFG = std::make_shared<ControlFlowGraph>();
    mCurrentCFG = pCFG.get();
    mCurrentBlock = mCurrentCFG->CreateNewBlock().get();

    mMod->InsertFunction(fDecl->GetFunctionName(), pCFG);

    // Function arguments were previously pushed on the stack during the call.
    // What we need to do now is associate each variable in the stack frame to registers
    size_t stackSlotsUsed = fDecl->GetParametersSize();
    unsigned currentStackSlot = 0;
    const ParamVarDecls* paramsDecl = fDecl->GetParametersDecl();
    for (auto& param : paramsDecl->GetParameters())
    {
        // TODO: This won't work When dealing with arrays and Strings because the info stored about them is an offset
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::LOAD }
                                         .AddRegOperand(GetOrInsertNodeRegister(param.get()))
                                         .AddStackSlotOperand(currentStackSlot++));
    }
    
    // Just being sure that every argument is correctly matched
    assert(currentStackSlot == stackSlotsUsed);

    HandleCompoundStmt(fDecl->GetBody());

    // Removing ties to the function
    mCurrentBlock = nullptr;
    mCurrentCFG = nullptr;
    mCurrentFunc = nullptr;
}

void InstructionSelector::HandleVarDecl(const ASTNode* decl) 
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(decl);
    assert(vDecl != nullptr);

    // For a function parameter nothing needs to be done, everything was handled in the function handling
    if (vDecl->IsFunctionParameter())
        return;

    const bool isGlobalVar = mSymTable->IsGlobalVariable(vDecl->GetName());
    const Expr* initExpr = vDecl->GetInitExpr();

    VirtualInstruction vInst;

    // Generate an instruction to load the initialization expression into the variable
    // TODO: Handle String and arrays
    if (initExpr != nullptr)
    {
        HandleExpr(initExpr);

        // Generate a load instruction into the variable's register
        vInst = VirtualInstruction{ VirtualInstruction::Opcode::MOV }
                                    .AddRegOperand(GetOrInsertNodeRegister(decl))
                                    .AddRegOperand(GetOrInsertNodeRegister(initExpr));
    }
    // If there's no initialization expression, the variable will be default initialized
    else
    {
        vInst = VirtualInstruction{ VirtualInstruction::Opcode::MOV }
                                    .AddRegOperand(GetOrInsertNodeRegister(decl))
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
                     .AddRegOperand(GetOrInsertNodeRegister(expr))
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
    case ASTNode::NodeKind::IDENTIFIER_EXPR:    // TODO: Is this useful?
    {
        const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(expr);
        assert(iExpr != nullptr);

        Symbol sym;
        mSymTable->GetLocalSymbol(mCurrentFunc->GetFunctionName(), iExpr->GetName(), mCurrentScopesTraversed, sym);

        auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::MOV }
                     .AddRegOperand(GetOrInsertNodeRegister(iExpr))
                     .AddRegOperand(1);     // TODO: Validate

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
                     .AddRegOperand(GetOrInsertNodeRegister(expr))
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
                     .AddRegOperand(GetOrInsertNodeRegister(expr))
                     .AddStackSlotOperand(memSlot);

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
    case Common::Opcode::NOT:   // TODO: This is a unary op
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
                                         .AddRegOperand(GetOrInsertNodeRegister(bExpr->GetLHS()))
                                         .AddRegOperand(GetOrInsertNodeRegister(bExpr->GetRHS())));
    }
    else
    {
        mMod->InsertGlobalVar(VirtualInstruction{ opcode }
                              .AddRegOperand(GetOrInsertNodeRegister(bExpr->GetLHS()))
                              .AddRegOperand(GetOrInsertNodeRegister(bExpr->GetRHS()))
                              .AddRegOperand(GetOrInsertNodeRegister(bExpr)));
    }
    
}

void InstructionSelector::HandleCallExpr(const ASTNode* expr) 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(expr);
    assert(cExpr != nullptr);
    unsigned nextStackSlot = 0;

    // Push the arguments on the stack in the function declaration order
    for (const auto& arg : cExpr->GetArgs())
    {
        const Expr* argExpr = dynamic_cast<const Expr*>(arg.get());
        HandleExpr(argExpr);
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::PUSH }
                                         .AddRegOperand(GetOrInsertNodeRegister(arg.get())));
        nextStackSlot++;
    }
    
    // If the function produce a value, we allocate some space on the stack for it
    Symbol fnSym;
    assert(mSymTable->GetGlobalSymbol(cExpr->GetCalleeName(), fnSym));
    if (fnSym.GetFunctionReturnType() != Common::Type::VOID)
    {
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::LOAD_SP }
                                         .AddImmOperand(nextStackSlot++));
    }

    // We generate a call instruction. This will take care of the stack pointer.
    mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::CALL }
                                     .AddTargetOperand(cExpr->GetCalleeName()));

    // We then pop the return value in a register if necessary
    if (fnSym.GetFunctionReturnType() != Common::Type::VOID)
    {
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::POP }
                                         .AddRegOperand(GetOrInsertNodeRegister(expr)));
    }
}

// Statements
BasicBlock* InstructionSelector::HandleCompoundStmt(const CompoundStmt* cStmt)
{
    CreateNewCurrentBlock();

    // Keeping a pointer to the first block of the compound statement.
    // This will be helpful for any caller that wants to link the compound statement with a control structure
    BasicBlock* entryBlock = mCurrentBlock;

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
    BasicBlock* thenBeginBlock = HandleCompoundStmt(iStmt->GetBody());
    
    // Keep a pointer to the current block which correspond to the end of the if body
    BasicBlock* thenEndBlock = mCurrentBlock;

    // Creating the exit block
    CreateNewCurrentBlock();
    
    // Generating a branch instruction that goes from the condition block to the then (body) begin block
    condBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                 .AddRegOperand(GetOrInsertNodeRegister(iStmt->GetCondExpr()))
                                 .AddTargetOperand(thenBeginBlock)
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

    const Expr* rExpr = rStmt->GetReturnExpr();

    if (rExpr != nullptr)
    {
        HandleExpr(rExpr);

        // A return value is transmitted to the caller through the stack
        mCurrentBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::PUSH }
                                         .AddRegOperand(mNextRegister - 1));
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
    BasicBlock* bodyBeginBlock = HandleCompoundStmt(wStmt->GetBody());

    // Keep a pointer to the current block which correspond to the end of the if body
    BasicBlock* bodyEndBlock = mCurrentBlock;

    // Creating the exit block
    CreateNewCurrentBlock();

    // Generating a branch instruction that goes from the header block to either the body begin block or the exit block
    headerBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                   .AddRegOperand(GetOrInsertNodeRegister(wStmt->GetCondExpr()))
                                   .AddTargetOperand(bodyBeginBlock)
                                   .AddTargetOperand(mCurrentBlock));

    // Generating a branch instruction from the body end block to the header block
    bodyEndBlock->InsertInstruction(VirtualInstruction{ VirtualInstruction::Opcode::JUMP }
                                    .AddTargetOperand(headerBlock));
}

void InstructionSelector::CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts)
{
    // Don't create a new block when the current one isn't properly terminated
    if (!mCurrentBlock->IsProperlyTerminated())
        return;

    BlockPtr newBlock = mCurrentCFG->CreateNewBlock(std::move(insts));
    mCurrentBlock->InsertBranch(newBlock);
    mCurrentBlock = newBlock.get();
}

unsigned InstructionSelector::GetOrInsertNodeRegister(const ASTNode* node)
{
    auto regIt = mNodeRegister.find(node);
    if (regIt == mNodeRegister.end())
    {
        auto nodeReg = mNodeRegister.insert({ node, mNextRegister++ });
        return nodeReg.first->second;
    }
    else
    {
        return regIt->second;
    }
}
