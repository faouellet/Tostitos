#include "interpreter.h"

#include "../AST/declarations.h"
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
    
    // Program entry is always the 'main' function
    const ASTNode* mainNode = mSymTable->FindFunctionDecl({ { Type::VOID }, 0, "main" });
    if (mainNode == nullptr)
    {
        // TODO: Log error
        return;
    }

    // There is no function calling 'main'
    mCallStack.push({ });

    HandleFunction(mainNode);
}

////////// Declarations //////////
void Interpreter::HandleFunction(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const FunctionDecl* fDecl = dynamic_cast<const FunctionDecl*>(this->mCurrentNode);
    assert(fDecl != nullptr);

    DispatchNode(fDecl->GetBody());
}

void Interpreter::HandleVarDecl(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
    assert(vDecl != nullptr);

    const Expr* initExpr = vDecl->GetInitExpr();

    // Since we're trying to assign a value to each node in the AST, we're not interested by uninitialized variable
    if (initExpr != nullptr)
    {
        DispatchNode(initExpr);
        InterpretedValue initVal;
        if (mCallStack.top().TryGetNodeValue(initExpr, initVal))
        {
            if (mSymTable->IsGlobalVariable(node))
            {
                mGlobalFrame.AddOrUpdateValue(node, initVal);
            }
            else
            {
                mCallStack.top().AddOrUpdateValue(node, initVal);
            }
        }
    }
}

////////// Expressions //////////
void Interpreter::HandleBinaryExpr(const FrontEnd::ASTNode* node) { }

void Interpreter::HandleBooleanExpr(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(this->mCurrentNode);
    assert(bExpr != nullptr);

    InterpretedValue bVal{ bExpr->GetValue() };
    mCallStack.top().AddOrUpdateValue(bExpr, bVal);
}

void Interpreter::HandleCallExpr(const FrontEnd::ASTNode* node) { }

void Interpreter::HandleIdentifierExpr(const FrontEnd::ASTNode* node) 
{
    //bool found;
    //Symbol* identSym;
    //std::tie(found, identSym) = mSymTable->TryGetSymbol(node);
    //if (found)
    //{
    //    if (identSym->IsGlobal())
    //    {
    //        mSymTable->AddVariableUse
    //
    //        mGlobalFrame.TryGetNodeValue()
    //    }
    //    else
    //    {
    //        mCallStack.top().
    //    }
    //}
}

void Interpreter::HandleNumberExpr(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(this->mCurrentNode);
    assert(nExpr != nullptr);

    InterpretedValue nVal{ nExpr->GetValue() };
    mCallStack.top().AddOrUpdateValue(nExpr, nVal);
}

void Interpreter::HandleStringExpr(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const StringExpr* sExpr = dynamic_cast<const StringExpr*>(this->mCurrentNode);
    assert(sExpr != nullptr);

    InterpretedValue sVal{ sExpr->GetName() };
    mCallStack.top().AddOrUpdateValue(sExpr, sVal);
}
     
////////// Statements //////////
void Interpreter::HandleCompoundStmt(const FrontEnd::ASTNode* node)
{
    mCurrentNode = node;

    const CompoundStmt* cStmt = dynamic_cast<const CompoundStmt*>(this->mCurrentNode);
    assert(cStmt != nullptr);

    for (const auto& stmt : cStmt->GetStatements())
        DispatchNode(stmt.get());
}

void Interpreter::HandleIfStmt(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const IfStmt* iStmt = dynamic_cast<const IfStmt*>(this->mCurrentNode);
    assert(iStmt != nullptr);

    const Expr* condExpr = iStmt->GetCondExpr();

    DispatchNode(condExpr);

    InterpretedValue condVal;
    assert(mCallStack.top().TryGetNodeValue(condExpr, condVal));
    if (condVal.GetBoolVal())
        DispatchNode(iStmt->GetBody());
}

void Interpreter::HandlePrintStmt(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(this->mCurrentNode);
    assert(pStmt != nullptr);

    const Expr* msgExpr = pStmt->GetMessage();
    if (msgExpr != nullptr)
    {
        InterpretedValue msgVal;
        if (mCallStack.top().TryGetNodeValue(msgExpr, msgVal))
        {
            std::cout << msgVal << std::endl;
        }

        // TODO: else log an error
    }
    else
    {
        // Printing a newline
        std::cout << std::endl;
    }
}

void Interpreter::HandleReturnStmt(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(this->mCurrentNode);
    assert(rStmt != nullptr);
}

void Interpreter::HandleScanStmt(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(this->mCurrentNode);
    assert(sStmt != nullptr);
}

void Interpreter::HandleWhileStmt(const FrontEnd::ASTNode* node) 
{
    mCurrentNode = node;

    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(this->mCurrentNode);
    assert(wStmt != nullptr);

    InterpretedValue condVal;
    do
    {
        const Expr* condExpr = wStmt->GetCondExpr();
        DispatchNode(condExpr);

        assert(mCallStack.top().TryGetNodeValue(condExpr, condVal));
        if (condVal.GetBoolVal())
            DispatchNode(wStmt->GetBody());
    } while (condVal.GetBoolVal());
}


void Interpreter::DispatchNode(const ASTNode* node)
{
    switch (node->GetKind())
    {
    case ASTNode::NodeKind::BINARY_EXPR:
        HandleBinaryExpr(node);
        break;
    case ASTNode::NodeKind::BOOLEAN_EXPR:
        HandleBooleanExpr(node);
        break;
    case ASTNode::NodeKind::CALL_EXPR:
        HandleCallExpr(node);
        break;
    case ASTNode::NodeKind::COMPOUND_STMT:
        HandleCompoundStmt(node);
        break;
    case ASTNode::NodeKind::FUNCTION_DECL:
        HandleFunction(node);
        break;
    case ASTNode::NodeKind::IDENTIFIER_EXPR:
        HandleIdentifierExpr(node);
        break;
    case ASTNode::NodeKind::IF_STMT:
        HandleIfStmt(node);
        break;
    case ASTNode::NodeKind::NUMBER_EXPR:
        HandleNumberExpr(node);
        break;
    case ASTNode::NodeKind::PRINT_STMT:
        HandlePrintStmt(node);
        break;
    case ASTNode::NodeKind::RETURN_STMT:
        HandleReturnStmt(node);
        break;
    case ASTNode::NodeKind::SCAN_STMT:
        HandleScanStmt(node);
        break;
    case ASTNode::NodeKind::STRING_EXPR:
        HandleStringExpr(node);
        break;
    case ASTNode::NodeKind::VAR_DECL:
        HandleVarDecl(node);
        break;
    case ASTNode::NodeKind::WHILE_STMT:
        HandleWhileStmt(node);
        break;
    default:
        assert(false); // TODO: Log an error instead?
        break;
    }
}
