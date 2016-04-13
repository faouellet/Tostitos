#include "scopechecker.h"

#include "../AST/declarations.h"
#include "../Utils/errorlogger.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

ScopeChecker::ScopeChecker()
    : mErrorCount{ 0 }, mCurrentScopeID{ 0 }, mCurrentFunc{ nullptr }
{
    mCurrentScopesTraversed.push_front(mCurrentScopeID);

    this->mPrologueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {
            mCurrentScopesTraversed.push_front(++mCurrentScopeID);
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
            mCurrentScopesTraversed.pop_front();
        }
        else if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            assert(mCurrentFunc != nullptr);
            mCurrentFunc = nullptr;
        }
    };
}

size_t ScopeChecker::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    mErrorCount = 0;
    mSymbolTable = symTab;
    this->VisitPostOrder(root);
    return mErrorCount;
}

void TosLang::FrontEnd::ScopeChecker::HandleCallExpr()
{
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);

    Symbol callSym;

    // Get the called function symbol. 
    // The first argument is an empty string because we want to fetch a global symbol (all functions are global symbols).
    if (!mSymbolTable->GetGlobalSymbol(cExpr->GetName(), callSym))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_UNDECLARED, cExpr->GetSourceLocation());
        ++mErrorCount;
    }
}

void ScopeChecker::HandleIdentifierExpr()
{
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(this->mCurrentNode);
    assert(iExpr != nullptr);

    Symbol varSym;
    bool foundSym = false;

    // Call the correct method for accessing the identifier symbol
    if (mCurrentFunc == nullptr)
        foundSym = mSymbolTable->GetGlobalSymbol(iExpr->GetName(), varSym);
    else
        foundSym = mSymbolTable->GetLocalSymbol(mCurrentFunc->GetName(), iExpr->GetName(), mCurrentScopesTraversed, varSym);

    if (!foundSym)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER, iExpr->GetSourceLocation());
        ++mErrorCount;
    }
}
