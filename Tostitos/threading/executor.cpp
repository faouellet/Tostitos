#include "executor.h"

#include "../../TosLang/AST/declarations.h"
#include "../../TosLang/Common/opcodes.h"
#include "../../TosLang/Common/type.h"
#include "../../TosLang/Sema/symboltable.h"

#include "threadutil.h"

#include <cassert>
#include <exception>    // TODO: Should we develop our own exception mechanism for Tostitos?
#include <iostream>     // TODO: Printing to standard IO for now. Should this be redirected to Tostitos later on?

using namespace Threading::impl;
using namespace TosLang;
using namespace TosLang::Common;
using namespace TosLang::FrontEnd;

Executor::Executor(const TosLang::FrontEnd::ASTNode* root,
    const TosLang::FrontEnd::SymbolTable* symTab)
    : mSymTable { symTab }, mCallStack{}
{
    // Create a call stack for the main thread then push the (empty for now) global 
    // frame onto it. This frame will contains all global scope level informations 
    // that are available to all the functions in the program. This make it the only
    // frame that can be accessed by any other.
    mCallStack.PushFrame({});
    mNextNodesToRun.push({});
    mNextNodesToRun.top().push_back(root);
}

Executor::Executor(const TosLang::FrontEnd::ASTNode* root,
                   const TosLang::FrontEnd::SymbolTable* symTab,
                   CallStack&& stack)
    : mSymTable { symTab }, mCallStack{ stack }
{
    mNextNodesToRun.push({});
    mNextNodesToRun.top().push_back(root);
}

bool Executor::ExecuteOne()
{
    if (mNextNodesToRun.empty())
    {
        return false;
    }

    DispatchNode(mNextNodesToRun.top().front());
    return true;
}

////////// Declarations //////////
void Executor::HandleFunction(const FrontEnd::ASTNode* node)
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(node);
    assert(fDecl != nullptr);

    // Popping the function node
    mNextNodesToRun.top().pop_front();

    // Fill it up with the content of the compound statement
    HandleCompoundStmt(fDecl->GetBody());
}

// We are using a pull model for variable initialization. What this means is that a variable will only 
// be initialized the first time it is encountered in the program. This will reduce the number of variables
// we need to keep track of at any given time. It also allows us to skip global variable initialization
// which would require another AST pass.
void Executor::HandleVarDecl(const FrontEnd::ASTNode* node)
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(node);
    assert(vDecl != nullptr);

    if (vDecl->IsFunctionParameter())
    {
        // Function paramters are not handled here
        return;
    }

    const Symbol* varSym = GetSymbol(node);
    const Expr* initExpr = vDecl->GetInitExpr();

    // If there's no initialization expression, the variable will 
    // be initialized with the default value for its type. Otherwise, mCurrentValue
    // should contains the initialization value
    InterpretedValue initVal;
    if (initExpr != nullptr)
    {
        if (!mCallStack.TryGetExprValue(initExpr, initVal))
        {
            mNextNodesToRun.top().push_front(initExpr);
            return;
        }
    }
    else
    {
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
    mNextNodesToRun.top().pop_front();
}

////////// Expressions //////////
void Executor::HandleArrayExpr(const FrontEnd::ASTNode* node)
{
    const ArrayExpr* aExpr = dynamic_cast<const ArrayExpr*>(node);
    assert(aExpr != nullptr);

    // TODO
    //aExpr->
}

void Executor::HandleBinaryExpr(const FrontEnd::ASTNode* node)
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(node);
    assert(bExpr != nullptr);
    
    InterpretedValue lhsval;
    if (!mCallStack.TryGetExprValue(bExpr->GetLHS(), lhsval))
    {
        mNextNodesToRun.top().push_front(bExpr->GetLHS());
        return;
    }

    InterpretedValue rhsval;
    if (!mCallStack.TryGetExprValue(bExpr->GetRHS(), lhsval))
    {
        mNextNodesToRun.top().push_front(bExpr->GetRHS());
        return;
    }

    // Both values must be usable at this point, if not, then there's a serious runtime error
    if (!lhsval.IsReady() || !rhsval.IsReady())
    {
        mNextNodesToRun.pop();
        return;
    }
    
    // Since type checking has been performed beforehand, we can assume that 
    // a certain operation only works with a certain type
    InterpretedValue binValue;
    switch (bExpr->GetOperation())
    {
    case Operation::ASSIGNMENT:
    {
        // Fetch the identifier symbol
        const Symbol* identSym = GetSymbol(bExpr->GetLHS());;

        mCallStack.AddOrUpdateSymbolValue(identSym, rhsval, identSym->IsGlobal());
    }
    case Operation::AND_BOOL:       binValue = InterpretedValue{ lhsval.GetBoolVal() && rhsval.GetBoolVal() }; break;
    case Operation::AND_INT:        binValue = InterpretedValue{ lhsval.GetIntVal() & rhsval.GetIntVal() }; break;
    case Operation::DIVIDE:         binValue = InterpretedValue{ lhsval.GetIntVal() / rhsval.GetIntVal() }; break;
    case Operation::EQUAL:          binValue = InterpretedValue{ lhsval.GetIntVal() == rhsval.GetIntVal() }; break;
    case Operation::GREATER_THAN:   binValue = InterpretedValue{ lhsval.GetIntVal() > rhsval.GetIntVal() }; break;
    case Operation::LEFT_SHIFT:     binValue = InterpretedValue{ lhsval.GetIntVal() << rhsval.GetIntVal() }; break;
    case Operation::LESS_THAN:      binValue = InterpretedValue{ lhsval.GetIntVal() < rhsval.GetIntVal() }; break;
    case Operation::MINUS:          binValue = InterpretedValue{ lhsval.GetIntVal() - rhsval.GetIntVal() }; break;
    case Operation::MODULO:         binValue = InterpretedValue{ lhsval.GetIntVal() % rhsval.GetIntVal() }; break;
    case Operation::MULT:           binValue = InterpretedValue{ lhsval.GetIntVal() && rhsval.GetIntVal() }; break;
    //TODO: case Operation::NOT:    binValue = InterpretedValue{ lhsval.GetBoolVal() && rhsval.GetBoolVal() }; break;
    case Operation::OR_BOOL:        binValue = InterpretedValue{ lhsval.GetBoolVal() || rhsval.GetBoolVal() }; break;
    case Operation::OR_INT:         binValue = InterpretedValue{ lhsval.GetIntVal() | rhsval.GetIntVal() }; break;
    case Operation::PLUS:           binValue = InterpretedValue{ lhsval.GetIntVal() + rhsval.GetIntVal() }; break;
    case Operation::RIGHT_SHIFT:    binValue = InterpretedValue{ lhsval.GetIntVal() >> rhsval.GetIntVal() }; break;
    default:                        binValue = {}; break;
    }

    mNextNodesToRun.top().pop_front();
    mCallStack.SetExprValue(bExpr, binValue, mCallStack.GetCurrentFrameID());
}

void Executor::HandleBooleanExpr(const FrontEnd::ASTNode* node)
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
    assert(bExpr != nullptr);

    mCallStack.SetExprValue(bExpr, InterpretedValue{ bExpr->GetValue() }, mCallStack.GetCurrentFrameID());
    mNextNodesToRun.top().pop_front();
}

void Executor::HandleCallExpr(const FrontEnd::ASTNode* node)
{
    // TODO: Make sure that no function can call the 'main' function

    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(node);
    assert(cExpr != nullptr);

    const auto& args = cExpr->GetArgs();
    if (!args.empty())
    {
        InterpretedValue argValue;

        if (!mCallStack.TryGetExprValue(args.front().get(), argValue))
        {
            for (const auto& arg : args)
                mNextNodesToRun.top().push_front(arg.get());

            return;
        }
        else
        {
            // Pushing a new frame on the call stack for the function call we're about to make
            mCallStack.PushFrame(PrepareNewFrame(cExpr));
        }
    }

    InterpretedValue callValue;
    if (!mCallStack.TryGetExprValue(cExpr, callValue))
    {
        const ASTNode* fnNode = mSymTable->GetFunctionDecl(cExpr);
        const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(fnNode);
        assert(fDecl != nullptr);

        // Setting up the function for execution
        mNextNodesToRun.top().pop_front();
        mNextNodesToRun.top().push_front(fDecl);
    }
    else
    {
        mCallStack.SetExprValue(cExpr, mCallStack.GetReturnValue(), mCallStack.GetCurrentFrameID());
        mNextNodesToRun.top().pop_front();
    }
}

void Executor::HandleIdentifierExpr(const FrontEnd::ASTNode* node)
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
        mNextNodesToRun.top().push_front(varDecl);
        return;
    }

    mCallStack.SetExprValue(node, identVal, mCallStack.GetCurrentFrameID());
}

void Executor::HandleIndexedExpr(const FrontEnd::ASTNode* node)
{
    const IndexedExpr* iExpr = dynamic_cast<const IndexedExpr*>(node);
    assert(iExpr != nullptr);

    // Get the index value
    InterpretedValue idxValue;
    if(mCallStack.TryGetExprValue(iExpr->GetIndex(), idxValue))
    {
        mNextNodesToRun.top().push_front(iExpr->GetIndex());
        return;
    }
    
    const int idx = idxValue.GetIntVal();;

    // Get the array to index
    InterpretedValue arrayValue;
    if (mCallStack.TryGetExprValue(iExpr->GetIdentifier(), arrayValue))
    {
        mNextNodesToRun.top().push_front(iExpr->GetIdentifier());
        return;
    }

    // We might have a runtime error if the index value doesn't fit in [0, array length[
    // TODO: bounds checking
    mCallStack.SetExprValue(iExpr, arrayValue[idx], mCallStack.GetCurrentFrameID());
}

void Executor::HandleNumberExpr(const FrontEnd::ASTNode* node)
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(node);
    assert(nExpr != nullptr);

    mNextNodesToRun.top().pop_front();
    mCallStack.SetExprValue(nExpr, InterpretedValue{ nExpr->GetValue() }, mCallStack.GetCurrentFrameID());
}

void Executor::HandleSpawnExpr(const FrontEnd::ASTNode* node)
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

    // TODO: Rethink this
    CreateThread(fDecl, mSymTable);
}

void Executor::HandleStringExpr(const FrontEnd::ASTNode* node)
{
    const StringExpr* sExpr = dynamic_cast<const StringExpr*>(node);
    assert(sExpr != nullptr);

    mCallStack.SetExprValue(sExpr, InterpretedValue{ sExpr->GetName() }, mCallStack.GetCurrentFrameID());
    mNextNodesToRun.top().pop_front();
}

////////// Statements //////////
void Executor::HandleCompoundStmt(const FrontEnd::ASTNode* node)
{
    const CompoundStmt* cStmt = dynamic_cast<const CompoundStmt*>(node);
    assert(cStmt != nullptr);

    // New queue for the new scope
    mNextNodesToRun.push({});

    for (const auto& stmt : cStmt->GetStatements())
        mNextNodesToRun.top().push_back(stmt.get());
}

void Executor::HandleIfStmt(const FrontEnd::ASTNode* node)
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(node);
    assert(iStmt != nullptr);

    const Expr* condExpr = iStmt->GetCondExpr();

    InterpretedValue condValue;
    assert(mCallStack.TryGetExprValue(condExpr, condValue));

    if (!condValue.IsReady())
    {
        mNextNodesToRun.pop();
        return;
    }

    mNextNodesToRun.top().pop_front();

    if (condValue.GetBoolVal())
        HandleCompoundStmt(iStmt->GetBody());
}

void Executor::HandlePrintStmt(const FrontEnd::ASTNode* node)
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(node);
    assert(pStmt != nullptr);

    const Expr* msgExpr = pStmt->GetMessage();
    if (msgExpr != nullptr)
    {
        InterpretedValue printValue;
        if (mCallStack.TryGetExprValue(msgExpr, printValue))
        {
            mNextNodesToRun.top().push_front(msgExpr);
            return;
        }

        if (!printValue.IsReady())
        {
            mNextNodesToRun.pop();
            return;
        }

        std::cout << printValue << std::endl;
    }
    else
    {
        // Printing a newline
        std::cout << std::endl;
    }

    mNextNodesToRun.top().pop_front();
}

void Executor::HandleReturnStmt(const FrontEnd::ASTNode* node)
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(node);
    assert(rStmt != nullptr);

    // There might be a return value
    const Expr* rExpr = rStmt->GetReturnExpr();
    InterpretedValue returnValue;
    if (!mCallStack.TryGetExprValue(rExpr, returnValue))
    {
        returnValue = InterpretedValue::CreateVoidValue();
    }

    // If we can, we place the return value in the caller's stack frame.
    // We can't do this when returning from the main function, since no function can call it.
    mCallStack.ExitCurrentFrame();
    if (!mCallStack.Empty() && rExpr != nullptr)
        mCallStack.SetReturnValue(returnValue);

    mNextNodesToRun.top().pop_front();
}

void Executor::HandleScanStmt(const FrontEnd::ASTNode* node)
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(node);
    assert(sStmt != nullptr);

    // TODO
}

void Executor::HandleSleepStmt(const FrontEnd::ASTNode* node)
{
    const SleepStmt* sStmt = dynamic_cast<const SleepStmt*>(node);
    assert(sStmt != nullptr);

    InterpretedValue sleepValue;
    assert(mCallStack.TryGetExprValue(sStmt->GetCountExpr(), sleepValue));

    if (!sleepValue.IsReady())
    {
        mNextNodesToRun.pop();
        return;
    }

    CurrentThreadSleepFor(sleepValue.GetIntVal());
    mNextNodesToRun.top().pop_front();
}

void Executor::HandleSyncStmt(const FrontEnd::ASTNode* node)
{
    const SyncStmt* sStmt = dynamic_cast<const SyncStmt*>(node);
    assert(sStmt != nullptr);

    CurrentThreadSync();
    mNextNodesToRun.top().pop_front();
}

void Executor::HandleWhileStmt(const FrontEnd::ASTNode* node)
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(node);
    assert(wStmt != nullptr);

    InterpretedValue condValue;
    if (!mCallStack.TryGetExprValue(wStmt->GetCondExpr(), condValue))
    {
        mNextNodesToRun.top().push_front(wStmt->GetCondExpr());
        return;
    }

    if (condValue.GetBoolVal())
    {
        // Push the condition node
        mNextNodesToRun.top().push_front(wStmt->GetCondExpr());

        // Push the body
        HandleCompoundStmt(wStmt->GetBody());
    }
    else
    {
        // We're done. Popping the while node
        mNextNodesToRun.top().pop_front();
    }
}

void Executor::DispatchNode(const ASTNode* node)
{
    assert(node != nullptr);

    switch (node->GetKind())
    {
    case ASTNode::NodeKind::BINARY_EXPR:        HandleBinaryExpr(node);
    case ASTNode::NodeKind::BOOLEAN_EXPR:       HandleBooleanExpr(node);
    case ASTNode::NodeKind::CALL_EXPR:          HandleCallExpr(node);
    case ASTNode::NodeKind::COMPOUND_STMT:      HandleCompoundStmt(node);
    case ASTNode::NodeKind::FUNCTION_DECL:      HandleFunction(node);
    case ASTNode::NodeKind::IDENTIFIER_EXPR:    HandleIdentifierExpr(node);
    case ASTNode::NodeKind::IF_STMT:            HandleIfStmt(node);
    case ASTNode::NodeKind::NUMBER_EXPR:        HandleNumberExpr(node);
    case ASTNode::NodeKind::PRINT_STMT:         HandlePrintStmt(node);
    case ASTNode::NodeKind::RETURN_STMT:        HandleReturnStmt(node);
    case ASTNode::NodeKind::SCAN_STMT:          HandleScanStmt(node);
    case ASTNode::NodeKind::STRING_EXPR:        HandleStringExpr(node);
    case ASTNode::NodeKind::VAR_DECL:           HandleVarDecl(node);
    case ASTNode::NodeKind::WHILE_STMT:         HandleWhileStmt(node);
    default:
        assert(false); // TODO: Log an error instead?
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
    // Setting the arguments
    std::vector<InterpretedValue> callVals;
    for (const auto& arg : call->GetArgs())
    {
        InterpretedValue argVal;
        assert(mCallStack.TryGetExprValue(arg.get(), argVal));

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

