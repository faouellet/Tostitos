#include "symbolcollector.h"

#include "../AST/declarations.h"

using namespace TosLang::FrontEnd;

SymbolCollector::SymbolCollector(const std::shared_ptr<SymbolTable>& symTab) : mCurrentScopeLevel{ 0 }, mSymbolTable{ symTab } 
{
    this->mPrologueFtr = [this]()
    { 
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {
            ++mCurrentScopeLevel;
        } 
    };

    this->mEpilogueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {
            --mCurrentScopeLevel;
        }
    };
}

void SymbolCollector::Collect(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPreOrder(root);
}

void SymbolCollector::HandleFunctionDecl() 
{
    const FunctionDecl* fnDecl = dynamic_cast<const FunctionDecl*>(mCurrentNode);
    assert(fnDecl != nullptr);

    Symbol fnSymbol{ Type::FUNCTION, mCurrentScopeLevel };


    mSymbolTable->AddSymbol(fnDecl->GetName(), fnSymbol);
}

void SymbolCollector::HandleParamVarDecl() 
{
}

void SymbolCollector::HandleVarDecl() 
{
    const VarDecl* varDecl = dynamic_cast<const VarDecl*>(mCurrentNode);
    assert(varDecl != nullptr);

    Symbol varSymbol{ Type::FUNCTION, mCurrentScopeLevel };


    mSymbolTable->AddSymbol(varDecl->GetName(), varSymbol);
}
