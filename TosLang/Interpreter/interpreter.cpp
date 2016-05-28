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
        // TODO: Log error
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
        mCallStack.front().AddOrUpdateValue(varSym, initVal);
    else
        mCallStack.back().AddOrUpdateValue(varSym, initVal);

    return{};
}

////////// Expressions //////////
InterpretedValue Interpreter::HandleBinaryExpr(const FrontEnd::ASTNode* node) 
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(node);
    assert(bExpr != nullptr);

    switch (bExpr->GetOperation())
    {
        case Operation::AND_BOOL:
            break;
        case Operation::AND_INT:
            break;
        case Operation::ASSIGNMENT:
            break;
        case Operation::DIVIDE:
            break;
        case Operation::GREATER_THAN:
            break;
        case Operation::LEFT_SHIFT:
            break;
        case Operation::LESS_THAN:
            break;
        case Operation::MINUS:
            break;
        case Operation::MODULO:
            break;
        case Operation::MULT:
            break;
        case Operation::NOT:
            break;
        case Operation::OR_BOOL:
            break;
        case Operation::OR_INT:
            break;
        case Operation::PLUS:
            break;
        case Operation::RIGHT_SHIFT:
            break;
        default:
            break;
    }

    return{};
}

InterpretedValue Interpreter::HandleBooleanExpr(const FrontEnd::ASTNode* node) 
{
    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(node);
    assert(bExpr != nullptr);

    return InterpretedValue{ bExpr->GetValue() };
}

InterpretedValue Interpreter::HandleCallExpr(const FrontEnd::ASTNode* node) { return{}; }

InterpretedValue Interpreter::HandleIdentifierExpr(const FrontEnd::ASTNode* node) 
{
    //bool found;
    //const Symbol* identSym;
    //std::tie(found, identSym) = mSymTable->TryGetSymbol(node);
    //assert(found);
    //
    //if (identSym->IsGlobal())
    //{
    //    mGlobalFrame.TryGetNodeValue();
    //}
    //else
    //{
    //    mCallStack.top().
    //}

    return{};
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

    for (const auto& stmt : cStmt->GetStatements())
        DispatchNode(stmt.get());

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
    case ASTNode::NodeKind::BINARY_EXPR:
        return HandleBinaryExpr(node);
    case ASTNode::NodeKind::BOOLEAN_EXPR:
        return HandleBooleanExpr(node);
    case ASTNode::NodeKind::CALL_EXPR:
        return HandleCallExpr(node);
    case ASTNode::NodeKind::COMPOUND_STMT:
        return HandleCompoundStmt(node);
    case ASTNode::NodeKind::FUNCTION_DECL:
        return HandleFunction(node);
    case ASTNode::NodeKind::IDENTIFIER_EXPR:
        return HandleIdentifierExpr(node);
    case ASTNode::NodeKind::IF_STMT:
        return HandleIfStmt(node);
    case ASTNode::NodeKind::NUMBER_EXPR:
        return HandleNumberExpr(node);
    case ASTNode::NodeKind::PRINT_STMT:
        return HandlePrintStmt(node);
    case ASTNode::NodeKind::RETURN_STMT:
        return HandleReturnStmt(node);
    case ASTNode::NodeKind::SCAN_STMT:
        return HandleScanStmt(node);
    case ASTNode::NodeKind::STRING_EXPR:
        return HandleStringExpr(node);
    case ASTNode::NodeKind::VAR_DECL:
        return HandleVarDecl(node);
    case ASTNode::NodeKind::WHILE_STMT:
        return HandleWhileStmt(node);
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