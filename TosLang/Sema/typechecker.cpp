#include "typechecker.h"

#include "../AST/declarations.h"
#include "../Utils/errorlogger.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
using namespace TosLang::Utils;

TypeChecker::TypeChecker(const std::shared_ptr<SymbolTable>& symTab) 
    : mErrorCount{ 0 }, mCurrentScopeID{ 0 }, mCurrentFunc{ nullptr }, mSymbolTable{ symTab }
{
    mCurrentScopesTraversed.push(mCurrentScopeID);

    this->mPrologueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {   
            mCurrentScopesTraversed.push(++mCurrentScopeID);
        }
        else if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            mCurrentFunc = dynamic_cast<FunctionDecl*>(mCurrentNode);
            assert(mCurrentFunc != nullptr);
        }
    };

    this->mEpilogueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {
            mCurrentScopesTraversed.pop();
        }
        else if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            assert(mCurrentFunc != nullptr);
            mCurrentFunc = nullptr;
        }
    };
}

unsigned TypeChecker::Run(const std::unique_ptr<ASTNode>& root)
{
    mErrorCount = 0;
    this->VisitPostOrder(root);
    return mErrorCount;
}

void TypeChecker::HandleVarDecl()
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
    assert(vDecl != nullptr);
        
    // We only perform a type check when a variable is declared AND initialized at the same time
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr());
    if (initExpr != nullptr)
    {
        Symbol varSymbol;
        std::string fnName = mCurrentFunc == nullptr ? "" : mCurrentFunc->GetName();
        if (!mSymbolTable->GetSymbol(fnName, vDecl->GetName(), mCurrentScopesTraversed, varSymbol))
        {
            // Variable wasn't declared
            ErrorLogger::PrintError(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER);
            ++mErrorCount;
            return;
        }

        if ((initExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR && varSymbol.mType != Type::BOOL)
            || (initExpr->GetKind() == ASTNode::NodeKind::NUMBER_EXPR && varSymbol.mType != Type::INT))
        {
            ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_LITERAL_TYPE);
            ++mErrorCount;
        }
        else
        {
            Symbol initSymbol;

            if (!mSymbolTable->GetSymbol(fnName, initExpr->GetName(), mCurrentScopesTraversed, initSymbol))
            {
                // TODO: More precise error message to dishtinguish the var and the init expr maybe?
                ErrorLogger::PrintError(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER);
                ++mErrorCount;
            }
        }
    }
}

void TosLang::FrontEnd::TypeChecker::HandleFunctionDecl()
{
    const FunctionDecl* fnDecl = dynamic_cast<const FunctionDecl*>(this->mCurrentNode);
    assert(fnDecl != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandleBinaryExpr()
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(this->mCurrentNode);
    assert(bExpr != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandleCallExpr()
{
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandleIfStmt()
{
    const IfStmt* ifStmt = dynamic_cast<const IfStmt*>(this->mCurrentNode);
    assert(ifStmt != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandlePrintStmt()
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(this->mCurrentNode);
    assert(pStmt != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandleScanStmt()
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(this->mCurrentNode);
    assert(sStmt != nullptr);
}

void TosLang::FrontEnd::TypeChecker::HandleWhileStmt()
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(this->mCurrentNode);
    assert(wStmt != nullptr);
}
