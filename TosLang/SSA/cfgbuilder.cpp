#include "cfgbuilder.h"

#include "ssafunction.h"
#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../Sema/symboltable.h"

#include <cassert>

// TODO: I've named way too much variables 'ssaInst'. Some renamings are in order.

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

std::unique_ptr<SSAModule> CFGBuilder::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTable)
{
    // Reset the state of the cfg builder
    mCurrentVarDef.clear();
    mIncompletePHIs.clear();
    mMod.reset(new SSAModule{});

    mSymTable = symTable;

    HandleProgramDecl(root);
    return std::move(mMod);
}

// Declarations
void CFGBuilder::HandleProgramDecl(const std::unique_ptr<ASTNode>& root)
{
    for (auto& stmt : root->GetChildrenNodes())
    {
        if (stmt->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
            HandleFunctionDecl(stmt.get());
        else if (stmt->GetKind() == ASTNode::NodeKind::VAR_DECL)
            HandleVarDecl(stmt.get());
        else
            // Shouldn't happen. If it does, it's because someone forgot to run the scope checker.
            assert(false && "Unknown declaration in program");  
    }
}

void CFGBuilder::HandleFunctionDecl(const ASTNode* decl)
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(decl);
    assert(fDecl != nullptr);
    
    // New function declaration so we need to build a new control flow graph
    FuncPtr pFuncPtr = std::make_shared<SSAFunction>();
    mCurrentFunction = pFuncPtr.get();
    mCurrentBlock = mCurrentFunction->CreateNewBlock().get();

    mMod->InsertFunction(fDecl->GetFunctionName(), pFuncPtr);

    // Associate each of the function argument with a SSA value
    const ParamVarDecls* paramsDecl = fDecl->GetParametersDecl();
    for (auto& param : paramsDecl->GetParameters())
    {
        // Fetch the parameter's symbol
        const Symbol* paramSymbol;
        bool symFound;
        std::tie(symFound, paramSymbol) = mSymTable->TryGetSymbol(param.get());
        assert(symFound);

        // TODO: Add type info to the SSA argument
        SSAValue ssaVal{ mNextID++ };
        mCurrentFunction->AddArguments(ssaVal);
        WriteVariable(paramSymbol, mCurrentBlock, 
                      mCurrentFunction->GetArgument(mCurrentFunction->GetNbArguments()));
    }

    HandleCompoundStmt(fDecl->GetBody());

    // Removing ties to the function
    mCurrentBlock = nullptr;
    mCurrentFunction = nullptr;
}

void CFGBuilder::HandleVarDecl(const ASTNode* decl) 
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(decl);
    assert(vDecl != nullptr);

    // Nothing to be done here for a function parameter
    if (vDecl->IsFunctionParameter())
        return;

    // TODO: Deal with global variable
    //const bool isGlobalVar = mSymTable->IsGlobalVariable(vDecl->GetName());
    const Expr* initExpr = vDecl->GetInitExpr();

    // Generate an instruction to load the initialization expression into the variable
    // TODO: Handle String and arrays
    if (initExpr != nullptr)
    {
        const SSAInstruction* initInst = HandleExpr(initExpr);

        const Symbol* varSym;
        bool symFound;
        std::tie(symFound, varSym) = mSymTable->TryGetSymbol(vDecl);
        assert(symFound);

        // Generate an assignment to the SSA variable
        WriteVariable(varSym, mCurrentBlock, initInst->GetReturnValue());
    }
}

// Expressions
const SSAInstruction* CFGBuilder::HandleExpr(const Expr* expr)
{
    const SSAInstruction* exprInst = nullptr;

    switch (expr->GetKind())
    {
    case ASTNode::NodeKind::BOOLEAN_EXPR:
    {
        const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(expr);
        assert(bExpr != nullptr);

        SSAInstruction ssaInst{ SSAInstruction::Operation::MOV, mNextID++, mCurrentBlock };
        ssaInst.AddOperand(SSAValue{ mNextID++, bExpr->GetValue() });
        
        exprInst = AddInstruction(ssaInst);
    }        
        break;
    case ASTNode::NodeKind::BINARY_EXPR:
        exprInst = HandleBinaryExpr(expr);
        break;
    case ASTNode::NodeKind::CALL_EXPR:
        HandleCallExpr(expr);
        // Since a function can't be called outside of another function, 
        // it is safe to get the terminator of the current basic block
        exprInst = mCurrentBlock->GetTerminator();
        break;
    case ASTNode::NodeKind::IDENTIFIER_EXPR:
    {
        bool symFound;
        const Symbol* identSym;
        std::tie(symFound, identSym) = mSymTable->TryGetSymbol(expr);
        assert(symFound);

        SSAInstruction ssaInst{ SSAInstruction::Operation::MOV, mNextID++, mCurrentBlock };
        ssaInst.AddOperand(ReadVariable(identSym, mCurrentBlock));

        exprInst = AddInstruction(ssaInst);
    }
        break;
    case ASTNode::NodeKind::NUMBER_EXPR:
    {
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(expr);
        assert(nExpr != nullptr);

        SSAInstruction ssaInst{ SSAInstruction::Operation::MOV, mNextID++, mCurrentBlock };
        ssaInst.AddOperand(SSAValue{ mNextID++, nExpr->GetValue() });
        
        exprInst = AddInstruction(ssaInst);
    }
        break;
    case ASTNode::NodeKind::STRING_EXPR:
    {
        // TODO: Redo completely
        //const StringExpr* sExpr = dynamic_cast<const StringExpr*>(expr);
        //assert(sExpr != nullptr);
        //
        //// We add the string literal to the module
        //unsigned memSlot = mMod->InsertArrayVariable(sExpr->GetName(), sExpr->GetName()); // Name and value are the same for simplicity
        //
        //// We generate a load of the string literal address
        //auto vInst = VirtualInstruction{ VirtualInstruction::Opcode::LOAD_IMM }
        //             .AddRegOperand(GetOrInsertNodeRegister(expr))
        //             .AddStackSlotOperand(memSlot);
        //
        //AddInstruction(ssaInst);
    }
        break;
    default:
        assert(false && "Unknown expression node");
        break;
    }

    return exprInst;
}

const SSAInstruction* CFGBuilder::HandleBinaryExpr(const ASTNode* expr)
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(expr);
    assert(bExpr != nullptr);

    // TODO: Assignment

    // Choose the correct opcode
    SSAInstruction::Operation op = SSAInstruction::Operation::UNKNOWN;
    switch (bExpr->GetOperation())
    {
    case Common::Operation::AND_BOOL:
    case Common::Operation::AND_INT:
        op = SSAInstruction::Operation::AND;
        break;
    case Common::Operation::ASSIGNMENT:
        op = SSAInstruction::Operation::MOV;
        break;
    case Common::Operation::DIVIDE:
        op = SSAInstruction::Operation::DIV;
        break;
    case Common::Operation::GREATER_THAN:
        op = SSAInstruction::Operation::GT;
        break;
    case Common::Operation::LEFT_SHIFT:
        op = SSAInstruction::Operation::LSHIFT;
        break;
    case Common::Operation::LESS_THAN:
        op = SSAInstruction::Operation::LT;
        break;
    case Common::Operation::MINUS:
        op = SSAInstruction::Operation::SUB;
        break;
    case Common::Operation::MODULO:
        op = SSAInstruction::Operation::MOD;
        break;
    case Common::Operation::MULT:
        op = SSAInstruction::Operation::MUL;
        break;
    case Common::Operation::NOT:   // TODO: This is a unary op
        op = SSAInstruction::Operation::NOT;
        break;
    case Common::Operation::OR_BOOL:
    case Common::Operation::OR_INT:
        op = SSAInstruction::Operation::OR;
        break;
    case Common::Operation::PLUS:
        op = SSAInstruction::Operation::ADD;
        break;
    case Common::Operation::RIGHT_SHIFT:
        op = SSAInstruction::Operation::RSHIFT;
        break;
    default:
        break;
    }

    // A match should have been found
    assert(op != SSAInstruction::Operation::UNKNOWN);

    // Handle the expression's operands
    const SSAInstruction* lhsInst = HandleExpr(bExpr->GetLHS());
    const SSAInstruction* rhsInst = HandleExpr(bExpr->GetRHS());
        
    SSAInstruction ssaInst{ op, mNextID++, mCurrentBlock };
    ssaInst.AddOperand(lhsInst->GetReturnValue());
    ssaInst.AddOperand(rhsInst->GetReturnValue());

    // Add the instruction to the program
    return AddInstruction(ssaInst);    
}

void CFGBuilder::HandleCallExpr(const ASTNode* expr) 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(expr);
    assert(cExpr != nullptr);

    // Generate a call instruction
    SSAInstruction callInst{ SSAInstruction::Operation::CALL, mNextID++, mCurrentBlock };

    // Add the values of its parameters
    for (const auto& arg : cExpr->GetArgs())
    {
        const Symbol* argSym;
        bool symFound;
        std::tie(symFound, argSym) = mSymTable->TryGetSymbol(arg.get());
        assert(symFound);
        callInst.AddOperand(ReadVariable(argSym, mCurrentBlock));
    }

    // TODO: Add a last operand for the return value if necessary
    
    mCurrentBlock->InsertInstruction(callInst);
}

// Statements
SSABlock* CFGBuilder::HandleCompoundStmt(const CompoundStmt* cStmt)
{
    CreateNewCurrentBlock();

    // Keeping a pointer to the first block of the compound statement.
    // This will be helpful for any caller that wants to link the compound statement with a control structure
    SSABlock* entryBlock = mCurrentBlock;

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

void CFGBuilder::HandleIfStmt(const ASTNode* stmt)
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(stmt);
    assert(iStmt != nullptr);

    // Generating instructions for the condition expression
    HandleExpr(iStmt->GetCondExpr());

    // Keeping a pointer to the condition block for later
    SSABlock* condBlock = mCurrentBlock;
    
    // Generating instructions for the if body
    SSABlock* thenBeginBlock = HandleCompoundStmt(iStmt->GetBody());
    
    // Keep a pointer to the current block which correspond to the end of the if body
    SSABlock* thenEndBlock = mCurrentBlock;

    // Creating the exit block
    CreateNewCurrentBlock();
    
    // Creating the branch instruction
    SSAInstruction brCondThenInst{ SSAInstruction::Operation::BR, mNextID++, mCurrentBlock };
    brCondThenInst.AddOperand(condBlock->GetTerminator()->GetReturnValue());
    condBlock->InsertInstruction(brCondThenInst);

    // Introduce branches to the condition block
    condBlock->InsertBranch(thenBeginBlock);
    condBlock->InsertBranch(mCurrentBlock);
    
    // Generating a branch instruction from the then (body) end block to the exit block
    SSAInstruction brThenExitInst{ SSAInstruction::Operation::BR, mNextID++, mCurrentBlock };
    brCondThenInst.AddOperand(condBlock->GetTerminator()->GetReturnValue());
    thenEndBlock->InsertInstruction(brCondThenInst);

    // Unconditional branch
    thenEndBlock->InsertBranch(mCurrentBlock);

    // TODO: Add PHIs to the join block
}

void CFGBuilder::HandlePrintStmt(const ASTNode* stmt)
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(stmt);
    assert(pStmt != nullptr);

    // TODO
}

void CFGBuilder::HandleReturnStmt(const ASTNode* stmt) 
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(stmt);
    assert(rStmt != nullptr);
    
    SSAInstruction retInst{ SSAInstruction::Operation::RET, mNextID++, mCurrentBlock };

    const Expr* rExpr = rStmt->GetReturnExpr();
    if (rExpr != nullptr)
    {
        const SSAInstruction* ssaInst = HandleExpr(rExpr);
        retInst.AddOperand(ssaInst->GetReturnValue());
    }

    mCurrentBlock->InsertInstruction(retInst);
}

void CFGBuilder::HandleScanStmt(const ASTNode* stmt)
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(stmt);
    assert(sStmt != nullptr);

    // TODO
}

void CFGBuilder::HandleWhileStmt(const ASTNode* stmt)
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(stmt);
    assert(wStmt != nullptr);

    // Creating the loop header (condition block)
    CreateNewCurrentBlock();

    // Generating instructions for the condition expression
    HandleExpr(wStmt->GetCondExpr());

    // Keeping a pointer to the header block for later
    SSABlock* headerBlock = mCurrentBlock;

    // Generating instructions for the while body
    SSABlock* bodyBeginBlock = HandleCompoundStmt(wStmt->GetBody());

    // Keep a pointer to the current block which correspond to the end of the if body
    SSABlock* bodyEndBlock = mCurrentBlock;

    // Creating the exit block (new value of mCurrentBlock)
    CreateNewCurrentBlock();

    // Creating the branch instruction
    SSAInstruction brHeaderBodyInst{ SSAInstruction::Operation::BR, mNextID++, mCurrentBlock };
    brHeaderBodyInst.AddOperand(headerBlock->GetTerminator()->GetReturnValue());
    headerBlock->InsertInstruction(brHeaderBodyInst);

    // Inserts branches going out of the condition block
    headerBlock->InsertBranch(bodyBeginBlock);
    headerBlock->InsertBranch(mCurrentBlock);

    // Inserts a branch instruction from the body end block to the header block
    bodyEndBlock->InsertBranch(headerBlock);
}

const SSAInstruction* CFGBuilder::AddInstruction(const SSAInstruction& inst)
{
    if (mCurrentBlock != nullptr)
    {
        mCurrentBlock->InsertInstruction(inst);
        return mCurrentBlock->GetTerminator();
    }
    else
    {
        return mMod->InsertGlobalVar(inst);
    }
}

void CFGBuilder::CreateNewCurrentBlock(SSAInstList&& insts)
{
    // An empty block is still good to go
    if (mCurrentBlock->GetNbInstructions() == 0)
        return;
    
    // Don't create a new block when the current one isn't properly terminated i.e. there's no branch at the end
    SSAInstruction::Operation lastInstOp = mCurrentBlock->GetTerminator()->GetOperation();
    if (lastInstOp == SSAInstruction::Operation::BR
        || lastInstOp == SSAInstruction::Operation::RET)
        return;

    SSABlockPtr newBlock = mCurrentFunction->CreateNewBlock(std::move(insts));
    mCurrentBlock->InsertBranch(newBlock);
    mCurrentBlock = newBlock.get();
}

void CFGBuilder::WriteVariable(const Symbol* variable, const SSABlock* block, const SSAValue& value)
{
    mCurrentVarDef[variable][block] = value;
}

SSAValue CFGBuilder::ReadVariable(const Symbol* variable, const SSABlock* block)
{
    auto varIt = mCurrentVarDef[variable].find(block);
    if (varIt != mCurrentVarDef[variable].end())
        return varIt->second;
    else
        return ReadVariableRecursive(variable, block);
}

SSAValue CFGBuilder::ReadVariableRecursive(const Symbol* variable, const SSABlock* block)
{
    SSAInstruction* ssaInst;
    SSAValue ssaVal;

    auto sealedIt = mSealedBlocks.find(block);
    if (sealedIt != mSealedBlocks.end())
    {
        // Incomplete CFG
        mCurrentBlock->InsertInstruction(SSAInstruction{ SSAInstruction::Operation::PHI, mNextID++, mCurrentBlock });
        ssaInst = mCurrentBlock->GetTerminator();
        mIncompletePHIs[block][variable] = ssaInst;
    }
    else if (mCurrentBlock->GetPredecessors().size() == 1)
    {
        // No PHI needed for a block with a single predecessor
        ssaVal = ReadVariable(variable, mCurrentBlock->GetPredecessors().front().get());
    }
    else
    {
        // Break potential cycles with operandless PHI
        mCurrentBlock->InsertInstruction(SSAInstruction{ SSAInstruction::Operation::PHI, mNextID++, mCurrentBlock });
        ssaInst = mCurrentBlock->GetTerminator();
        WriteVariable(variable, block, ssaVal);
        ssaVal = AddPHIOperand(variable, ssaInst);
    }

    WriteVariable(variable, block, ssaVal);

    return ssaVal;
}

SSAValue CFGBuilder::AddPHIOperand(const Symbol* variable, SSAInstruction* phi)
{
    for (auto predIt = phi->GetBlock()->pred_begin(), predEnd = phi->GetBlock()->pred_end(); predIt != predEnd; ++predIt)
        phi->AddOperand(ReadVariable(variable, predIt->get()));

    return TryRemoveTrivialPHI(phi);
}

SSAValue CFGBuilder::TryRemoveTrivialPHI(SSAInstruction* phi)
{
    const SSAValue none{};
    SSAValue same{};

    for (auto& op : phi->GetOperands())
    {
        if ((op == same) || (op == phi->GetReturnValue()))
            continue;   // Unique value or self-reference
        if (same != none)
            return phi->GetReturnValue(); // The phi merges at least two values: not trivial
        same = op;
    }

    // If we are here, it's because phi has been proven trivial and can be 
    // replaced by the one value it had to decide on
    SSAInstruction newInst{ SSAInstruction::Operation::MOV, mNextID++, mCurrentBlock };
    newInst.AddOperand(phi->GetReturnValue());
    
    // Reroute all uses of phi to same 
    for (auto& use : phi->GetUsers())
        newInst.AddUser(use);
    
    // Remove phi
    SSABlock* block = phi->GetBlock();
    block->ReplaceInstruction(*phi, newInst);
    
    // Try to recursively remove all phi users, which might have become trivial
    for (auto& use : newInst.GetUsers())
        if (use->GetOperation() == SSAInstruction::Operation::PHI)
            TryRemoveTrivialPHI(use);

    return same;
}
