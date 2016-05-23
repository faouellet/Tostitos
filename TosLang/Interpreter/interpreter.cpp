#include "interpreter.h"

#include "../AST/declarations.h"
#include "../Common/type.h"
#include "../Sema/symboltable.h"

#include <cassert>

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

    for (const auto& stmt : fDecl->GetBody()->GetStatements())
        DispatchNode(stmt.get());
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

void Interpreter::HandleIdentifierExpr(const FrontEnd::ASTNode* node) { }

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
void Interpreter::HandleIfStmt(const FrontEnd::ASTNode* node) { }

void Interpreter::HandlePrintStmt(const FrontEnd::ASTNode* node) { }

void Interpreter::HandleReturnStmt(const FrontEnd::ASTNode* node) { }

void Interpreter::HandleScanStmt(const FrontEnd::ASTNode* node) { }

void Interpreter::HandleWhileStmt(const FrontEnd::ASTNode* node) { }


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
