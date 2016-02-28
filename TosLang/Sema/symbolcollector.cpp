#include "symbolcollector.h"

#include "../Utils/errorlogger.h"

#include <sstream>
#include <type_traits>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
using namespace TosLang::Utils;

SymbolCollector::SymbolCollector(const std::shared_ptr<SymbolTable>& symTab) 
    : mCurrentScopeLevel{ 0 }, mCurrentScopeID{ 0 }, mErrorCount{ 0 }, mCurrentFunc{ nullptr }, mSymbolTable{ symTab }
{
    // TODO: This probably needs some refactoring since the same code is also present in the type checker
    this->mPrologueFtr = [this]()
    { 
        if (mCurrentNode != nullptr)
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
        }
    };

    this->mEpilogueFtr = [this]()
    {
        if (mCurrentNode != nullptr)
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
        }
    };
}

size_t SymbolCollector::Run(const std::unique_ptr<ASTNode>& root)
{
    mErrorCount = 0;
    mSymbolTable->Clear();
    mCurrentScopeID = 0;

    this->VisitPreOrder(root);

    assert(mCurrentFunc == nullptr);
    assert(mCurrentScopeLevel == M_GLOBAL_SCOPE_LEVEL);

    return mErrorCount;
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
        assert(paramVar != nullptr);
        // Since Common::Type is an enum class, we need to do a cast to its underlying type
        sStream << static_cast<std::underlying_type<Common::Type>::type>(paramVar->GetVarType());

        fnType.push_back(paramVar->GetVarType());
    }

    // Add the function symbol to the table
    if (!mSymbolTable->AddGlobalSymbol(sStream.str(), { fnType, 0, fnDecl->GetName() }))
    {
        // Couldn't insert the function in the symbol table because it's trying to redefine another function
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_REDEFINITION, fnDecl->GetSourceLocation());
        ++mErrorCount;
    }
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
        assert(paramVar != nullptr);
        paramTypes.push_back(paramVar->GetVarType());

        // Add the parameter to the symbols defined in the scope of the current function
        if (!mSymbolTable->AddLocalSymbol(mCurrentFunc->GetName(), paramVar->GetVarName(), { paramVar->GetVarType(), mCurrentScopeID + 1, paramVar->GetName() }))
        {
            // Couldn't insert the param in the symbol table because it's trying to redefine another param
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PARAM_REDEFINITION, paramVar->GetSourceLocation());
            ++mErrorCount;
        }
    }
}

void SymbolCollector::HandleVarDecl() 
{
    const VarDecl* varDecl = dynamic_cast<const VarDecl*>(mCurrentNode);
    assert(varDecl != nullptr);

    // This method is only for handling variables that aren't function parameters
    if (varDecl->IsFunctionParameter())
        return;
    
    // If this a function parameter, it will have been already handled by HandleParamVarDecl
    if (varDecl->IsFunctionParameter())
        return;

    bool isNotAlreadyDefined = false;
    if (mCurrentScopeLevel == M_GLOBAL_SCOPE_LEVEL)
        isNotAlreadyDefined  = mSymbolTable->AddGlobalSymbol(varDecl->GetName(), { varDecl->GetVarType(), 0, varDecl->GetName() });
    else
        isNotAlreadyDefined = mSymbolTable->AddLocalSymbol(mCurrentFunc->GetName(), varDecl->GetName(), { varDecl->GetVarType(), mCurrentScopeID, varDecl->GetName() });

    if (!isNotAlreadyDefined)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_REDEFINITION, varDecl->GetSourceLocation());
        ++mErrorCount;
    }
}
