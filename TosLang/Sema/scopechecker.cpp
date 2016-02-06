#include "scopechecker.h"

#include "../Utils/errorlogger.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

TosLang::FrontEnd::ScopeChecker::ScopeChecker(const std::shared_ptr<SymbolTable>& symTab)
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

size_t TosLang::FrontEnd::ScopeChecker::Run(const std::unique_ptr<ASTNode>& root)
{
    mErrorCount = 0;
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
    if (!mSymbolTable->GetSymbol(std::string{}, cExpr->GetName(), mCurrentScopesTraversed, callSym))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_UNDECLARED, cExpr->GetSourceLocation());
        ++mErrorCount;
    }
}

void TosLang::FrontEnd::ScopeChecker::HandleIdentifierExpr()
{
    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(this->mCurrentNode);
    assert(iExpr != nullptr);

    Symbol varSym;
    std::string fnName = mCurrentFunc == nullptr ? "" : mCurrentFunc->GetName();
    if (!mSymbolTable->GetSymbol(fnName, iExpr->GetName(), mCurrentScopesTraversed, varSym))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER, iExpr->GetSourceLocation());
        ++mErrorCount;
    }
}
