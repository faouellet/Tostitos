#include "llvmgenerator.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

static llvm::Type* ConvertToLLVMType(const TosLang::Common::Type type);

std::unique_ptr<llvm::Module> LLVMGenerator::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    // Reset the state of the instruction selector
    mMod.reset(new llvm::Module{"TosLang Module", llvm::getGlobalContext()});
    mSymTable = symTab;

    HandleProgramDecl(root);
    return std::move(mMod);
}

// Declarations
std::unique_ptr<llvm::Module> LLVMGenerator::HandleProgramDecl(const std::unique_ptr<ASTNode>& root)
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

    return std::move(mMod);
}

llvm::Function* LLVMGenerator::HandleFunctionDecl(const ASTNode* decl)
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(decl);
    assert(fDecl != nullptr);

    // Generate code for the function declaration
    const ParamVarDecls* paramsDecl = fDecl->GetParametersDecl();
    std::vector<llvm::Type*> argTypes;
    argTypes.reserve(paramsDecl->GetParameters().size());

    for (auto& param : paramsDecl->GetParameters())
    {
        const VarDecl* vDecl = dynamic_cast<const VarDecl*>(param.get());
        argTypes.emplace_back(ConvertToLLVMType(vDecl->GetVarType()));
    }

    llvm::Type* returnType = ConvertToLLVMType(fDecl->GetReturnType());

    llvm::FunctionType* fType = llvm::FunctionType::get(returnType, argTypes, false);
    llvm::Function* func = llvm::Function::Create(fType, llvm::Function::ExternalLinkage, fDecl->GetName(), mMod.get());

    // Create a basic block to start inserting into
    llvm::BasicBlock* bb = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
    mBuilder.SetInsertPoint(bb);

    // Handle the function arguments
    mNamedValues.clear();
    for (auto& arg : func->args())
    {
        // Allocate space on the function stack frame
        llvm::AllocaInst* allocaInst = mBuilder.CreateAlloca(arg.getType(), nullptr, arg.getName());

        // Store the argument into the alloca
        mBuilder.CreateStore(&arg, allocaInst);

        // Add argument to the generator symbol table
        mNamedValues[arg.getName()] = allocaInst;
    }

    llvm::Value* retVal = HandleCompoundStmt(fDecl->GetBody());

    // Error generating body, erase the function from the module
    if (retVal == nullptr)
        func->eraseFromParent();

    mBuilder.CreateRet(retVal);

    return func;
}

void LLVMGenerator::HandleVarDecl(const ASTNode* decl) 
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
llvm::Value* LLVMGenerator::HandleExpr(const Expr* expr)
{
    switch (expr->GetKind())
    {
    case ASTNode::NodeKind::BINARY_EXPR:
    {
        return HandleBinaryExpr(expr);
    }
    case ASTNode::NodeKind::BOOLEAN_EXPR:
    {
        const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(expr);
        assert(bExpr != nullptr);

        return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, bExpr->GetValue()));
    }
    case ASTNode::NodeKind::CALL_EXPR:
    {
        return HandleCallExpr(expr);
    }
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

        return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(16, nExpr->GetValue()));
    }
    case ASTNode::NodeKind::STRING_EXPR:
    {
        const StringExpr* sExpr = dynamic_cast<const StringExpr*>(expr);
        assert(sExpr != nullptr);

        return llvm::constant

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

llvm::Value* LLVMGenerator::HandleBinaryExpr(const ASTNode* expr)
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(expr);
    assert(bExpr != nullptr);

    // Handle the expression's operands
    llvm::Value* lhsExpr = HandleExpr(bExpr->GetLHS());
    llvm::Value* rhsExpr = HandleExpr(bExpr->GetRHS());

    // TODO: Assignment

    // Choose the correct opcode
    switch (bExpr->GetOperation())
    {
    case Common::Opcode::AND_BOOL:
    case Common::Opcode::AND_INT:
        return mBuilder.CreateAnd(lhsExpr, rhsExpr, "andtmp");
    case Common::Opcode::DIVIDE:
        return mBuilder.CreateUDiv(lhsExpr, rhsExpr, "divtmp");
    case Common::Opcode::GREATER_THAN:
        return mBuilder.CreateICmpUGT(lhsExpr, rhsExpr, "gttmp");
    case Common::Opcode::LEFT_SHIFT:
        return mBuilder.CreateShl(lhsExpr, rhsExpr, "shltmp");
    case Common::Opcode::LESS_THAN:
        return mBuilder.CreateICmpULE(lhsExpr, rhsExpr, "lttmp");
    case Common::Opcode::MINUS:
        return mBuilder.CreateSub(lhsExpr, rhsExpr, "subtmp");
    case Common::Opcode::MODULO:
        return mBuilder.CreateSRem(lhsExpr, rhsExpr, "remtmp");
    case Common::Opcode::MULT:
        return mBuilder.CreateMul(lhsExpr, rhsExpr, "multmp");
    case Common::Opcode::NOT:
        // TODO
        break;
    case Common::Opcode::OR_BOOL:
    case Common::Opcode::OR_INT:
        return mBuilder.CreateOr(lhsExpr, rhsExpr, "ortmp");
    case Common::Opcode::PLUS:
        return mBuilder.CreateAdd(lhsExpr, rhsExpr, "addtmp");
    case Common::Opcode::RIGHT_SHIFT:
        return mBuilder.CreateAShr(lhsExpr, rhsExpr, "shrtmp");
    default:
        break;
    }    
}

llvm::Value* LLVMGenerator::HandleCallExpr(const ASTNode* expr) 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(expr);
    assert(cExpr != nullptr);

    // Generate the list of values to be passed to the called function
    std::vector<llvm::Value*> argsValue;
    for (const auto& arg : cExpr->GetArgs())
    {
        const Expr* argExpr = dynamic_cast<const Expr*>(arg.get());
        argsValue.emplace_back(HandleExpr(argExpr));
    }
    
    return mBuilder.CreateCall(mMod->getFunction(cExpr->GetCalleeName()), argsValue, "calltmp");
}

// Statements
llvm::Value* LLVMGenerator::HandleCompoundStmt(const CompoundStmt* cStmt)
{
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
}

llvm::Value* LLVMGenerator::HandleIfStmt(const ASTNode* stmt)
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(stmt);
    assert(iStmt != nullptr);

    // Generating instructions for the condition expression
    llvm::Value* condVal = HandleExpr(iStmt->GetCondExpr());

    // Convert condition value to bool by comparing it to 0
    mBuilder.CreateICmpNE(condVal, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(16, 0)), "ifcond");

    llvm::Function* currentFunc = mBuilder.GetInsertBlock()->getParent();

    // Create blocks for the then compound statement and for if continuation
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", currentFunc);
    llvm::BasicBlock* contBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "ifcont");

    // Create the branching
    mBuilder.CreateCondBr(condVal, thenBB, contBB);

    // Generating instructions for the if body
    mBuilder.SetInsertPoint(thenBB);
    llvm::Value* thenVal = HandleCompoundStmt(iStmt->GetBody());
    mBuilder.CreateBr(contBB);

    thenBB = mBuilder.GetInsertBlock(); // Compound statement code generation can change the current basic block

    // Insert the continuation block in the function
    currentFunc->getBasicBlockList().push_back(contBB);
    
    // Emit PHI nodes in the continuation block
    mBuilder.SetInsertPoint(contBB);
    llvm::PHINode* pNode = mBuilder.CreatePHI(llvm::Type::getInt16Ty(llvm::getGlobalContext()), 1, "iftmp");

    pNode->addIncoming(thenVal, thenBB);

    return pNode;
}

void LLVMGenerator::HandlePrintStmt(const ASTNode* stmt)
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(stmt);
    assert(pStmt != nullptr);

    // TODO
}

void LLVMGenerator::HandleReturnStmt(const ASTNode* stmt) 
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

void LLVMGenerator::HandleScanStmt(const ASTNode* stmt)
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(stmt);
    assert(sStmt != nullptr);

    // TODO
}

llvm::Value* LLVMGenerator::HandleWhileStmt(const ASTNode* stmt)
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

llvm::Type* ConvertToLLVMType(const TosLang::Common::Type type)
{
    switch (type)
    {
    case TosLang::Common::Type::BOOL:
        return llvm::Type::getInt8Ty(llvm::getGlobalContext());
    case TosLang::Common::Type::NUMBER:
        return llvm::Type::getInt16Ty(llvm::getGlobalContext());
    default:
        // TODO: Deal with arrays and strings
        return nullptr;
    }
}
