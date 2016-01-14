#include "symbolcollector.h"

#include <sstream>
#include <type_traits>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

SymbolCollector::SymbolCollector(const std::shared_ptr<SymbolTable>& symTab) 
    : mCurrentScopeLevel{ 0 }, mCurrentScopeID{ 0 }, mCurrentFunc{ nullptr }, mSymbolTable{ symTab }
{
    // TODO: This probably needs some refactoring since the same code is also present in the type checker
    this->mPrologueFtr = [this]()
    { 
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::COMPOUND_STMT)
        {
            ++mCurrentScopeID;
            ++mCurrentScopeLevel;
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
            --mCurrentScopeLevel;
        }
        else if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            assert(mCurrentFunc != nullptr);
            mCurrentFunc = nullptr;
        }
    };
}

void SymbolCollector::Run(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPreOrder(root);
}

void SymbolCollector::HandleFunctionDecl() 
{
    const FunctionDecl* fnDecl = dynamic_cast<const FunctionDecl*>(mCurrentNode);
    assert(fnDecl != nullptr);

    std::vector<Type> fnType;

    // The symbol name for a function is a concatenation of its name, its return type and the type of its arguments
    std::stringstream sStream;
    sStream << fnDecl->GetName();
    sStream << static_cast<std::underlying_type<Common::Type>::type>(fnDecl->GetReturnType());
    fnType.push_back(fnDecl->GetReturnType());

    const ParamVarDecls* paramDecl = dynamic_cast<const ParamVarDecls*>(fnDecl->GetArguments());
    assert(paramDecl != nullptr);

    for (auto& param : paramDecl->GetParameters())
    {
        const VarDecl* paramVar = dynamic_cast<const VarDecl*>(param.get());
        // Since Common::Type is an enum class, we need to do a cast to its underlying type
        sStream << static_cast<std::underlying_type<Common::Type>::type>(paramVar->GetVarType());

        fnType.push_back(paramVar->GetVarType());
    }

    mSymbolTable->AddGlobalSymbol(sStream.str(), { fnType, 0 });
}

void SymbolCollector::HandleParamVarDecl() 
{
    const ParamVarDecls* paramDecl = dynamic_cast<const ParamVarDecls*>(mCurrentNode);
    assert(paramDecl != nullptr);

    std::vector<Common::Type> paramTypes;
    for (auto& param : paramDecl->GetParameters())
    {
        // Add the parameter type to the list of the function's parameters' types
        const VarDecl* paramVar = dynamic_cast<const VarDecl*>(param.get());
        paramTypes.push_back(paramVar->GetVarType());

        // Add the parameter to the symbols defined in the scope of the current function
        mSymbolTable->AddLocalSymbol(mCurrentFunc->GetName(), paramVar->GetVarName(), { paramVar->GetVarType(), mCurrentScopeID + 1 });
    }
}

void SymbolCollector::HandleVarDecl() 
{
    const VarDecl* varDecl = dynamic_cast<const VarDecl*>(mCurrentNode);
    assert(varDecl != nullptr);

    if (mCurrentScopeLevel == M_GLOBAL_SCOPE_LEVEL)
        mSymbolTable->AddGlobalSymbol(varDecl->GetName(), { varDecl->GetVarType(), 0 });
    else
        mSymbolTable->AddLocalSymbol(mCurrentFunc->GetName(), varDecl->GetName(), { varDecl->GetVarType(), mCurrentScopeID });
}
