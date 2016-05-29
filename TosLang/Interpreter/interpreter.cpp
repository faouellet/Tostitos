#include "interpreter.h"

#include "../AST/declarations.h"
#include "../Common/opcodes.h"
#include "../Common/type.h"
#include "../Sema/symboltable.h"

#include <cassert>
#include <iostream> // TODO: Printing to standard IO for now. Should this be redirected to Tostitos later on?

using namespace TosLang;
using namespace TosLang::Common;
using namespace TosLang::FrontEnd;

void Interpreter::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab) 
{
    mSymTable = symTab;
    mCallStack.clear();
    
    // Program entry is always the 'main' function
    const ASTNode* mainNode = mSymTable->FindFunctionDecl({ { Type::VOID }, 0, "main" });
    if (mainNode == nullptr)
    {
        // TODO: Log error and add a unit test for it
        return;
    }

    // Push the global frame onto the call stack
    // This frame will contains all global scope level informations that are available to all the functions in the program.
    // This make it the only frame that can be accessed by any other.
    mCallStack.push_back({ });

    HandleFunction(mainNode);
}

////////// Declarations //////////
InterpretedValue Interpreter::HandleFunction(const FrontEnd::ASTNode* node) 
{
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(node);
    assert(fDecl != nullptr);

    return DispatchNode(fDecl->GetBody());
}

// We are using a pull model for variable initialization. What this means is that a variable will only 
// be initialized the first time it is encountered in the program. This will reduce the number of variables
// we need to keep track of at any given time. It also allows us to skip global variable initialization
// which would require another AST pass.
InterpretedValue Interpreter::HandleVarDecl(const FrontEnd::ASTNode* node) 
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(node);
    assert(vDecl != nullptr);

    if(vDecl->IsFunctionParameter())
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

    if (varSym->IsGlobal())
        mCallStack.front().AddOrUpdateSymbolValue(varSym, initVal);
    else
        mCallStack.back().AddOrUpdateSymbolValue(varSym, initVal);

    return initVal;
}

////////// Expressions //////////
InterpretedValue Interpreter::HandleBinaryExpr(const FrontEnd::ASTNode* node) 
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

        if (identSym->IsGlobal())
            mCallStack.front().AddOrUpdateSymbolValue(identSym, rhsval);
        else
            mCallStack.back().AddOrUpdateSymbolValue(identSym, rhsval);

        return rhsval;
    }
    case Operation::AND_BOOL:       return InterpretedValue{ lhsval.GetBoolVal() && rhsval.GetBoolVal() };
    case Operation::AND_INT:        return InterpretedValue{ lhsval.GetIntVal() & rhsval.GetIntVal() };
    case Operation::DIVIDE:         return InterpretedValue{ lhsval.GetIntVal() / rhsval.GetIntVal() };
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

InterpretedValue Interpreter::HandleBooleanExpr(const FrontEnd::ASTNode* node) 
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
    assert(bExpr != nullptr);

    return InterpretedValue{ bExpr->GetValue() };
}

InterpretedValue Interpreter::HandleCallExpr(const FrontEnd::ASTNode* node) 
{ 
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(node);
    assert(cExpr != nullptr);
     
    // Evaluating all the argument expressions to find out the values the function is being called with
    std::vector<InterpretedValue> callVals;
    std::vector<Common::Type> fnTypes;
    for (const auto& arg : cExpr->GetArgs())
    {
        InterpretedValue argVal{ DispatchNode(arg.get()) };

        callVals.push_back(argVal);

        // While we're at it, we also record the type of the argument.
        // This is done to help us find the function being called later on
        switch (argVal.GetType())
        {
        case InterpretedValue::ValueType::BOOLEAN:  
            fnTypes.push_back(Common::Type::BOOL);
            break;
        case InterpretedValue::ValueType::INTEGER:
            fnTypes.push_back(Common::Type::NUMBER);
            break;
        case InterpretedValue::ValueType::STRING:
            fnTypes.push_back(Common::Type::STRING);
            break;
        default:
            assert(false);  // Something went wrong when evaluating the argument's value
            break;
        }
    }

    // We need to find the function that is being called
    Symbol expectedFnSym{ fnTypes, 0, cExpr->GetCalleeName() };
    const ASTNode* fnNode = mSymTable->FindFunctionDecl(expectedFnSym);
    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(fnNode);
    assert(fDecl != nullptr);

    // Pushing a new frame on the call stack for the function we're about to call
    mCallStack.push_back({});

    // Initializing the function's parameters in the new stack frame
    const ParamVarDecls* pVDecls = fDecl->GetParametersDecl();
    assert(pVDecls->GetParameters().size() == callVals.size());
    for (size_t iArg = 0; iArg < callVals.size(); ++iArg)
    {
        const Symbol* argSym = GetSymbol(pVDecls->GetParameters()[iArg].get());
        mCallStack.back().AddOrUpdateSymbolValue(argSym, callVals[iArg]);
    }

    return{}; 
}

InterpretedValue Interpreter::HandleIdentifierExpr(const FrontEnd::ASTNode* node) 
{
    const Symbol* identSym = GetSymbol(node);
    InterpretedValue identVal;
    bool foundVal = false;

    if (identSym->IsGlobal())
        foundVal = mCallStack.front().TryGetSymbolValue(identSym, identVal);
    else
        foundVal = mCallStack.back().TryGetSymbolValue(identSym, identVal);

    // Since we're working with a pull model, it is possible that we had yet to encounter this variable.
    // In that case, we just need to evaluate the associated variable declaration.
    if (!foundVal)
    {
        const ASTNode* varDecl = mSymTable->FindVarDecl(node, mCurrentScope);
        identVal = HandleVarDecl(varDecl);
    }

    return identVal;
}

InterpretedValue Interpreter::HandleNumberExpr(const FrontEnd::ASTNode* node) 
{
    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(node);
    assert(nExpr != nullptr);

    return InterpretedValue{ nExpr->GetValue() };
}

InterpretedValue Interpreter::HandleStringExpr(const FrontEnd::ASTNode* node) 
{
    const StringExpr* sExpr = dynamic_cast<const StringExpr*>(node);
    assert(sExpr != nullptr);

    return InterpretedValue{ sExpr->GetName() };
}
     
////////// Statements //////////
InterpretedValue Interpreter::HandleCompoundStmt(const FrontEnd::ASTNode* node)
{
    const CompoundStmt* cStmt = dynamic_cast<const CompoundStmt*>(node);
    assert(cStmt != nullptr);

    ++mCurrentScope;

    for (const auto& stmt : cStmt->GetStatements())
        DispatchNode(stmt.get());

    --mCurrentScope;
    
    return{};
}

InterpretedValue Interpreter::HandleIfStmt(const FrontEnd::ASTNode* node) 
{
    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(node);
    assert(iStmt != nullptr);

    const Expr* condExpr = iStmt->GetCondExpr();

    InterpretedValue condVal{ DispatchNode(condExpr) };

    if (condVal.GetBoolVal())
        DispatchNode(iStmt->GetBody());

    return{};
}

InterpretedValue Interpreter::HandlePrintStmt(const FrontEnd::ASTNode* node) 
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

InterpretedValue Interpreter::HandleReturnStmt(const FrontEnd::ASTNode* node) 
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(node);
    assert(rStmt != nullptr);

    const Expr* rExpr = rStmt->GetReturnExpr();
    if (rExpr != nullptr)
        return DispatchNode(rExpr);
    else
        return{};
}

InterpretedValue Interpreter::HandleScanStmt(const FrontEnd::ASTNode* node) 
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(node);
    assert(sStmt != nullptr);

    return{};
}

InterpretedValue Interpreter::HandleWhileStmt(const FrontEnd::ASTNode* node) 
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


InterpretedValue Interpreter::DispatchNode(const ASTNode* node)
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

const Symbol* Interpreter::GetSymbol(const ASTNode* node) const
{
    const Symbol* varSym;
    bool found;
    std::tie(found, varSym) = mSymTable->TryGetSymbol(node);
    assert(found);
    return varSym;
}
