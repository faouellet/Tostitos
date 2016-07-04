#include "executor.h"

#include "../../TosLang/AST/declarations.h"
#include "../../TosLang/Common/opcodes.h"
#include "../../TosLang/Common/type.h"
#include "../../TosLang/Sema/symboltable.h"

#include "../../Tostitos/threading/threadutil.h"

#include <cassert>
#include <iostream> // TODO: Printing to standard IO for now. Should this be redirected to Tostitos later on?

using namespace Threading::impl;
using namespace TosLang;
using namespace TosLang::Common;
using namespace TosLang::FrontEnd;

Executor::Executor(const TosLang::FrontEnd::ASTNode* root,
                   const TosLang::FrontEnd::SymbolTable* symTab,
                   CallStack&& stack)
    : mDoneWithCurrentFunc{ false }, mCurrentNode{ root }, mSymTable{ symTab }, mCallStack{ stack } { }

void Executor::Run()
{
    if (mCurrentNode == nullptr)
    {
        return;
    }

    HandleFunction(mCurrentNode);
}

////////// Declarations //////////
InterpretedValue Executor::HandleFunction(const FrontEnd::ASTNode* node)
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(node);
    assert(fDecl != nullptr);

    DispatchNode(fDecl->GetBody());

    // Done with callee but not with caller
    mDoneWithCurrentFunc = false;

    // If we're returning from 'main', we won't have a callstack to fetch the return value from
    if (mCallStack.Empty())
        return{};
    else
        return mCallStack.GetReturnValue();
}

// We are using a pull model for variable initialization. What this means is that a variable will only 
// be initialized the first time it is encountered in the program. This will reduce the number of variables
// we need to keep track of at any given time. It also allows us to skip global variable initialization
// which would require another AST pass.
InterpretedValue Executor::HandleVarDecl(const FrontEnd::ASTNode* node)
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(node);
    assert(vDecl != nullptr);

    if (vDecl->IsFunctionParameter())
    {
        // Function paramters are not handled here
        return{};
    }

    const Symbol* varSym = GetSymbol(node);
    const Expr* initExpr = vDecl->GetInitExpr();

    // Initialize the variable with the value of the initialization expression if possible
    InterpretedValue initVal;
    if (initExpr != nullptr)
    {
        initVal = DispatchNode(initExpr);
    }
    else
    {
        // If there's no initialization expression, the variable will 
        // be initialized with the default value for its type
        switch (varSym->GetVariableType())
        {
        case Type::BOOL:
            initVal = InterpretedValue{ false };
            break;
        case Type::NUMBER:
            initVal = InterpretedValue{ 0 };
            break;
        case Type::STRING:
            initVal = InterpretedValue{ "" };
            break;
        default:
            assert(false); // What is this variable?
            break;
        }
    }

    mCallStack.AddOrUpdateSymbolValue(varSym, initVal, varSym->IsGlobal());

    return initVal;
}

////////// Expressions //////////
InterpretedValue Executor::HandleArrayExpr(const FrontEnd::ASTNode* node)
{
    const ArrayExpr* aExpr = dynamic_cast<const ArrayExpr*>(node);
    assert(aExpr != nullptr);

    //aExpr->

    return{};
}

InterpretedValue Executor::HandleBinaryExpr(const FrontEnd::ASTNode* node)
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(node);
    assert(bExpr != nullptr);

    InterpretedValue lhsval{ DispatchNode(bExpr->GetLHS()) };
    InterpretedValue rhsval{ DispatchNode(bExpr->GetRHS()) };

    // Since type checking has been performed beforehand, we can assume that 
    // a certain operation only works with a certain type
    switch (bExpr->GetOperation())
    {
    case Operation::ASSIGNMENT:
    {
        // Fetch the identifier symbol
        const Symbol* identSym = GetSymbol(bExpr->GetLHS());;

        mCallStack.AddOrUpdateSymbolValue(identSym, rhsval, identSym->IsGlobal());

        return rhsval;
    }
    case Operation::AND_BOOL:       return InterpretedValue{ lhsval.GetBoolVal() && rhsval.GetBoolVal() };
    case Operation::AND_INT:        return InterpretedValue{ lhsval.GetIntVal() & rhsval.GetIntVal() };
    case Operation::DIVIDE:         return InterpretedValue{ lhsval.GetIntVal() / rhsval.GetIntVal() };
    case Operation::EQUAL:          return InterpretedValue{ lhsval.GetIntVal() == rhsval.GetIntVal() };
    case Operation::GREATER_THAN:   return InterpretedValue{ lhsval.GetIntVal() > rhsval.GetIntVal() };
    case Operation::LEFT_SHIFT:     return InterpretedValue{ lhsval.GetIntVal() << rhsval.GetIntVal() };
    case Operation::LESS_THAN:      return InterpretedValue{ lhsval.GetIntVal() < rhsval.GetIntVal() };
    case Operation::MINUS:          return InterpretedValue{ lhsval.GetIntVal() - rhsval.GetIntVal() };
    case Operation::MODULO:         return InterpretedValue{ lhsval.GetIntVal() % rhsval.GetIntVal() };
    case Operation::MULT:           return InterpretedValue{ lhsval.GetIntVal() && rhsval.GetIntVal() };
                                    //TODO: case Operation::NOT:            return InterpretedValue{ lhsval.GetBoolVal() && rhsval.GetBoolVal() };
    case Operation::OR_BOOL:        return InterpretedValue{ lhsval.GetBoolVal() || rhsval.GetBoolVal() };
    case Operation::OR_INT:         return InterpretedValue{ lhsval.GetIntVal() | rhsval.GetIntVal() };
    case Operation::PLUS:           return InterpretedValue{ lhsval.GetIntVal() + rhsval.GetIntVal() };
    case Operation::RIGHT_SHIFT:    return InterpretedValue{ lhsval.GetIntVal() >> rhsval.GetIntVal() };
    default:                        return{};
    }
}

InterpretedValue Executor::HandleBooleanExpr(const FrontEnd::ASTNode* node)
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
    assert(bExpr != nullptr);

    return InterpretedValue{ bExpr->GetValue() };
}

InterpretedValue Executor::HandleCallExpr(const FrontEnd::ASTNode* node)
{
    // TODO: Make sure that no function can call the 'main' function

    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(node);
    assert(cExpr != nullptr);

    // Pushing a new frame on the call stack for the function call we're about to make
    mCallStack.PushFrame(PrepareNewFrame(cExpr));

    const ASTNode* fnNode = mSymTable->GetFunctionDecl(cExpr);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(fnNode);
    assert(fDecl != nullptr);

    // Executing the call
    return HandleFunction(fDecl);
}

InterpretedValue Executor::HandleIdentifierExpr(const FrontEnd::ASTNode* node)
{
    const Symbol* identSym = GetSymbol(node);
    InterpretedValue identVal;
    bool foundVal = false;

    foundVal = mCallStack.TryGetSymbolValue(identSym, identVal, identSym->IsGlobal());

    // Since we're working with a pull model, it is possible that we had yet to encounter this variable.
    // In that case, we just need to evaluate the associated variable declaration.
    if (!foundVal)
    {
        const ASTNode* varDecl = mSymTable->GetVarDecl(node);
        identVal = HandleVarDecl(varDecl);
    }

    return identVal;
}

InterpretedValue Executor::HandleIndexedExpr(const FrontEnd::ASTNode* node)
{
    const IndexedExpr* iExpr = dynamic_cast<const IndexedExpr*>(node);
    assert(iExpr != nullptr);

    // Get the index value
    InterpretedValue indexVal{ DispatchNode(iExpr->GetIndex()) };
    const int idx = indexVal.GetIntVal();

    // Get the array to index
    InterpretedValue arrayVal{ DispatchNode(iExpr->GetIdentifier()) };

    // We might have a runtime error if the index value doesn't fit in [0, array length[
    //if (idx < 0 || arrayVal.)
    {
        // TODO: Handle it
    }

    return{};
}

InterpretedValue Executor::HandleNumberExpr(const FrontEnd::ASTNode* node)
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(node);
    assert(nExpr != nullptr);

    return InterpretedValue{ nExpr->GetValue() };
}

InterpretedValue Executor::HandleSpawnExpr(const FrontEnd::ASTNode* node)
{
    const SpawnExpr* sExpr = dynamic_cast<const SpawnExpr*>(node);
    assert(sExpr != nullptr);

    // The call stack for a thread will contain two things:
    // 1- The global frame
    // 2- The frame of the function being called
    CallStack stack;
    stack.PushFrame(mCallStack.GetGlobalFrame());
    stack.PushFrame(PrepareNewFrame(sExpr->GetCall()));

    // We need to find the function that is being called
    const ASTNode* fnNode = mSymTable->GetFunctionDecl(sExpr->GetCall());
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(fnNode);
    assert(fDecl != nullptr);

    //Threads::Kernel::GetInstance().CreateThread({ fnNode, mSymTable, std::move(stack) });

    return InterpretedValue{ };
}

InterpretedValue Executor::HandleStringExpr(const FrontEnd::ASTNode* node)
{
    const StringExpr* sExpr = dynamic_cast<const StringExpr*>(node);
    assert(sExpr != nullptr);

    return InterpretedValue{ sExpr->GetName() };
}

////////// Statements //////////
InterpretedValue Executor::HandleCompoundStmt(const FrontEnd::ASTNode* node)
{
    const CompoundStmt* cStmt = dynamic_cast<const CompoundStmt*>(node);
    assert(cStmt != nullptr);

    for (const auto& stmt : cStmt->GetStatements())
        if (!mDoneWithCurrentFunc)
            DispatchNode(stmt.get());

    return{};
}

InterpretedValue Executor::HandleIfStmt(const FrontEnd::ASTNode* node)
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(node);
    assert(iStmt != nullptr);

    const Expr* condExpr = iStmt->GetCondExpr();

    InterpretedValue condVal{ DispatchNode(condExpr) };

    if (condVal.GetBoolVal())
        DispatchNode(iStmt->GetBody());

    return{};
}

InterpretedValue Executor::HandlePrintStmt(const FrontEnd::ASTNode* node)
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(node);
    assert(pStmt != nullptr);

    const Expr* msgExpr = pStmt->GetMessage();
    if (msgExpr != nullptr)
    {
        InterpretedValue msgVal{ DispatchNode(msgExpr) };
        std::cout << msgVal << std::endl;
    }
    else
    {
        // Printing a newline
        std::cout << std::endl;
    }

    return{};
}

InterpretedValue Executor::HandleReturnStmt(const FrontEnd::ASTNode* node)
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(node);
    assert(rStmt != nullptr);

    // There might be a return value
    const Expr* rExpr = rStmt->GetReturnExpr();
    InterpretedValue returnVal = InterpretedValue::CreateVoidValue();

    if (rExpr != nullptr)
        returnVal = DispatchNode(rExpr);

    // And we're done here
    mDoneWithCurrentFunc = true;

    // If we can, we place the return value in the caller's stack frame.
    // We can't do this when returning from the main function, since no function can call it.
    mCallStack.ExitCurrentFrame();
    if (!mCallStack.Empty())
        mCallStack.SetReturnValue(returnVal);

    return returnVal;
}

InterpretedValue Executor::HandleScanStmt(const FrontEnd::ASTNode* node)
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(node);
    assert(sStmt != nullptr);

    // TODO

    return{};
}

InterpretedValue Executor::HandleSleepStmt(const FrontEnd::ASTNode* node)
{
    const SleepStmt* sStmt = dynamic_cast<const SleepStmt*>(node);
    assert(sStmt != nullptr);

    InterpretedValue timeVal = DispatchNode(sStmt->GetCountExpr());

    //Threads::Kernel::GetInstance().SleepFor(timeVal.GetIntVal());

    return{};
}

InterpretedValue Executor::HandleSyncStmt(const FrontEnd::ASTNode* node)
{
    const SyncStmt* sStmt = dynamic_cast<const SyncStmt*>(node);
    assert(sStmt != nullptr);

    Sync();

    return{};
}

InterpretedValue Executor::HandleWhileStmt(const FrontEnd::ASTNode* node)
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(node);
    assert(wStmt != nullptr);

    InterpretedValue condVal;
    do
    {
        const Expr* condExpr = wStmt->GetCondExpr();
        condVal = DispatchNode(condExpr);

        if (condVal.GetBoolVal())
            DispatchNode(wStmt->GetBody());
    } while (condVal.GetBoolVal());

    return{};
}

InterpretedValue Executor::DispatchNode(const ASTNode* node)
{
    assert(node != nullptr);

    switch (node->GetKind())
    {
    case ASTNode::NodeKind::BINARY_EXPR:        return HandleBinaryExpr(node);
    case ASTNode::NodeKind::BOOLEAN_EXPR:       return HandleBooleanExpr(node);
    case ASTNode::NodeKind::CALL_EXPR:          return HandleCallExpr(node);
    case ASTNode::NodeKind::COMPOUND_STMT:      return HandleCompoundStmt(node);
    case ASTNode::NodeKind::FUNCTION_DECL:      return HandleFunction(node);
    case ASTNode::NodeKind::IDENTIFIER_EXPR:    return HandleIdentifierExpr(node);
    case ASTNode::NodeKind::IF_STMT:            return HandleIfStmt(node);
    case ASTNode::NodeKind::NUMBER_EXPR:        return HandleNumberExpr(node);
    case ASTNode::NodeKind::PRINT_STMT:         return HandlePrintStmt(node);
    case ASTNode::NodeKind::RETURN_STMT:        return HandleReturnStmt(node);
    case ASTNode::NodeKind::SCAN_STMT:          return HandleScanStmt(node);
    case ASTNode::NodeKind::STRING_EXPR:        return HandleStringExpr(node);
    case ASTNode::NodeKind::VAR_DECL:           return HandleVarDecl(node);
    case ASTNode::NodeKind::WHILE_STMT:         return HandleWhileStmt(node);
    default:
        assert(false); // TODO: Log an error instead?
        return{};
    }
}

const Symbol* Executor::GetSymbol(const ASTNode* node) const
{
    const Symbol* varSym;
    bool found;
    std::tie(found, varSym) = mSymTable->TryGetSymbol(node);
    assert(found);
    return varSym;
}

StackFrame Executor::PrepareNewFrame(const TosLang::FrontEnd::CallExpr* call)
{
    // Evaluating all the argument expressions to find out the values the function is being called with
    std::vector<InterpretedValue> callVals;
    for (const auto& arg : call->GetArgs())
    {
        InterpretedValue argVal{ DispatchNode(arg.get()) };

        callVals.push_back(argVal);
    }

    // We need to find the function that is being called
    const ASTNode* fnNode = mSymTable->GetFunctionDecl(call);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(fnNode);
    assert(fDecl != nullptr);

    // Initializing the function's parameters in the new stack frame
    StackFrame frame;
    const ParamVarDecls* pVDecls = fDecl->GetParametersDecl();
    assert(pVDecls->GetParameters().size() == callVals.size());
    for (size_t iArg = 0; iArg < callVals.size(); ++iArg)
    {
        const Symbol* argSym = GetSymbol(pVDecls->GetParameters()[iArg].get());
        frame.AddOrUpdateSymbolValue(argSym, callVals[iArg]);
    }

    return frame;
}

