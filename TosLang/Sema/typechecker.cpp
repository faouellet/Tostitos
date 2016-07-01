#include "typechecker.h"

#include "../AST/declarations.h"
#include "../Sema/symboltable.h"
#include "../Utils/errorlogger.h"

#include <algorithm>
#include <cassert>
#include <iterator>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
using namespace TosLang::Utils;

TypeChecker::TypeChecker() 
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
            mCurrentFunc = static_cast<const FunctionDecl*>(mCurrentNode);
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

size_t TypeChecker::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    // Cleanup before starting a new run
    mErrorCount = 0;
    mSymbolTable = symTab;
    mNodeTypes.clear();
    mOverloadMap.clear();

    this->VisitPostOrder(root);

    // Some calls may still be unresolved because of lack of context at their call sites 
    // (mostly due to an ignored returned value). We try to resolve these calls by going over
    // their overload sets and checking if there's a valid function candidate. If there's 
    // more than one, we have a problem. If there's none we should already have flagged the problem.
    for (const auto& callOverloads : mOverloadMap)
    {
        if (callOverloads.second.size() == 1)
        {
            const CallExpr* cExpr = dynamic_cast<const CallExpr*>(callOverloads.first);
            mSymbolTable->AddFunctionUse(cExpr, *callOverloads.second.front());
        }
        else if (!callOverloads.second.empty())
        {
            // TODO: Log an error
            ++mErrorCount;
        }
    }

    return mErrorCount;
}

bool TypeChecker::CheckExprEvaluateToType(const Expr* expr, Type type)
{
    // In the most common case, we already know the type of the expression
    if ((expr->GetKind() != ASTNode::NodeKind::BINARY_EXPR) 
        && (expr->GetKind() != ASTNode::NodeKind::CALL_EXPR))
    {
        return mNodeTypes.at(expr) == type;
    }

    // TODO: unit test with binary expression containing multiple function calls

    // However, there are two cases when the node type might still be undecided
    // 1- A call expression will still have an undecided type at this point because 
    //    we're still not done with its overload resolution. We currently know of 
    //    every potential function candidates that share its argument types. All that's 
    //    left to do is the final matching by matching the expected return type.
    // 2- A binary expression containing call expression(s) won't have its type decided 
    //    until the call expression(s) are finally resolved
    if (expr->GetKind() == ASTNode::NodeKind::CALL_EXPR)
    {
        const CallExpr* cExpr = static_cast<const CallExpr*>(expr);
        assert(cExpr != nullptr);

        // The overload set contains all functions that have matching parameters.
        // Now, we need to complete the overload resolution of the function call 
        // by finding a function that also has a matching return type.
        auto oSetIt = mOverloadMap.find(cExpr);
        if (oSetIt == mOverloadMap.end())
            return false;
        
        const std::vector<const Symbol*>& overloadSet = oSetIt->second;
        if (overloadSet.empty())
            return false;

        auto matchIt = std::find_if(overloadSet.begin(), overloadSet.end(),
                                    [&type](const Symbol* sym)
                                    {
                                        return sym->GetFunctionReturnType() == type;
                                    });

        if (matchIt != overloadSet.end())
        {
            mSymbolTable->AddFunctionUse(cExpr, **matchIt);

            // No need for the overload set anymore
            mOverloadMap.erase(oSetIt);

            return true;
        }
        else
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_RETURN_ERROR,
                                              mCurrentNode->GetSourceLocation());
            ++mErrorCount;
            return false;
        }
    }
    else    // Binary expression
    {
        const BinaryOpExpr* bExpr = static_cast<const BinaryOpExpr*>(expr);
        assert(bExpr != nullptr);

        // We force the evaluation of the binary expression operands if necessary. This happens when 
        // one of the operands is a function call. It is reflected in the type checker by the binary
        // expression node not having any associated type
        auto bExprIt = mNodeTypes.find(bExpr);
        if (bExprIt != mNodeTypes.end())
            return bExprIt->second == type;
         
        Type typeForEval = type;
        // A '<' or a '>' requires numbers to be compared to one another
        if ((bExpr->GetOperation() == Operation::GREATER_THAN) || (bExpr->GetOperation() == Operation::LESS_THAN))
            typeForEval = Type::NUMBER;
        
        bool evalutesToType = CheckExprEvaluateToType(bExpr->GetLHS(), typeForEval);
        evalutesToType &= CheckExprEvaluateToType(bExpr->GetRHS(), typeForEval);
        
        if (evalutesToType)
        {
            mNodeTypes[expr] = type;
            return true;
        }
        else
        {
            // TODO: Log an error?
            return false;
        }
    }
}

void TypeChecker::HandleVarDecl()
{
    const VarDecl* vDecl = static_cast<const VarDecl*>(this->mCurrentNode);
    assert(vDecl != nullptr);
        
    // We only perform a type check when a variable is declared AND initialized at the same time
    const Expr* initExpr = static_cast<const Expr*>(vDecl->GetInitExpr());
    if (initExpr == nullptr)
        return;
     
    const Symbol* varSymbol;
    bool symFound;
    std::tie(symFound, varSymbol) = mSymbolTable->TryGetSymbol(vDecl);

    assert(symFound);

    if (!CheckExprEvaluateToType(initExpr, varSymbol->GetVariableType()))
    {
        switch (initExpr->GetKind())
        {
        case ASTNode::NodeKind::ARRAY_EXPR:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_INIT_SCALAR_ARRAY, initExpr->GetSourceLocation());
            ++mErrorCount;
            break;
        case ASTNode::NodeKind::BOOLEAN_EXPR:
        case ASTNode::NodeKind::NUMBER_EXPR:
        case ASTNode::NodeKind::STRING_EXPR:
            if(vDecl->GetVarSize() == 0)
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_LITERAL_TYPE, initExpr->GetSourceLocation());
            else
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_INIT_ARRAY_SCALAR, initExpr->GetSourceLocation());
            ++mErrorCount;
            break;
        case ASTNode::NodeKind::BINARY_EXPR:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_EXPR_TYPE, initExpr->GetSourceLocation());
            ++mErrorCount;
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_VARIABLE_TYPE, mCurrentNode->GetSourceLocation());
            ++mErrorCount;
            break;
        }
    }

    // If we're dealing with an array, we need to check that the array length matches the number of elements in the array expression
    if ((vDecl->GetVarSize() != 0) && (initExpr->GetKind() == ASTNode::NodeKind::ARRAY_EXPR))
    {
        const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(initExpr);
        assert(aExpr != nullptr);

        if (vDecl->GetVarSize() != aExpr->GetChildrenNodes().size())
        {
            // TODO: Log an error and test it
            ++mErrorCount;
            return;
        }
    }
    
}

void TypeChecker::HandleArrayExpr()
{
    const ArrayExpr* aExpr = static_cast<const ArrayExpr*>(this->mCurrentNode);
    assert(aExpr != nullptr);

    const ChildrenNodes& children = aExpr->GetChildrenNodes();

    // An empty array expression doesn't make any sense
    if (children.empty())
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::ARRAY_EMPTY_EXPR, aExpr->GetSourceLocation());
        ++mErrorCount;
        return;
    }

    // Every element in the array expression must have the same type
    const Common::Type arrayType = mNodeTypes[children.front().get()];
    for (const auto& arrayElem : children)
    {
        if (mNodeTypes[arrayElem.get()] != arrayType)
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::ARRAY_MULTIPLE_TYPES, aExpr->GetSourceLocation());
            ++mErrorCount;
            return;
        }
    }

    mNodeTypes[aExpr] = GetArrayVersion(arrayType);
}

void TypeChecker::HandleBooleanExpr()
{
    mNodeTypes[this->mCurrentNode] = Type::BOOL;
}

void TypeChecker::HandleBinaryExpr()
{
    const BinaryOpExpr* bExpr = static_cast<const BinaryOpExpr*>(this->mCurrentNode);
    assert(bExpr != nullptr);
    
    const ChildrenNodes& children = bExpr->GetChildrenNodes();
    assert(children.size() == 2);
    
    // An assignment only makes sense when the left hand side operand is an identifier i.e. a modifiable lvalue
    if ((children.front()->GetKind() != ASTNode::NodeKind::IDENTIFIER_EXPR) 
        && (bExpr->GetOperation() == Operation::ASSIGNMENT))
    {
        // TODO: Log an error and add a unit test for it
        ++mErrorCount;
        return;
    }

    Type operandTypes[2];
    for (int i = 0; i < 2; ++i)
    {
        switch (children[i]->GetKind())
        {
        case ASTNode::NodeKind::BINARY_EXPR:
            operandTypes[i] = mNodeTypes.at(children[i].get());
            break;
        case ASTNode::NodeKind::BOOLEAN_EXPR:
            operandTypes[i] = Type::BOOL;
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
        {
            const IdentifierExpr* iExpr = static_cast<const IdentifierExpr*>(children[i].get());
            // It is assumed that the scope checker has been run before the type checker
            const Symbol* varSym;
            bool symFound;
            std::tie(symFound, varSym) = mSymbolTable->TryGetSymbol(iExpr);
            assert(symFound);
            operandTypes[i] = varSym->GetVariableType();
        }
            break;
        case ASTNode::NodeKind::NUMBER_EXPR:
            operandTypes[i] = Type::NUMBER;
            break;
        }
    }

    // Check if the operands' types match
    if (operandTypes[0] != operandTypes[1])
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_BIN_EXPR_TYPE, bExpr->GetSourceLocation());
        ++mErrorCount;
        return;
    }

    if ((bExpr->GetOperation() == Operation::EQUAL) || 
        (bExpr->GetOperation() == Operation::GREATER_THAN) || 
        (bExpr->GetOperation() == Operation::LESS_THAN))
        // When using '<', '>' or '=', the resulting type will be different than the operand types.
        // It is using number expressions to produce a boolean expression.
        mNodeTypes[bExpr] = Type::BOOL;
    else
        mNodeTypes[bExpr] = operandTypes[0];
}

void TypeChecker::HandleCallExpr()
{
    const CallExpr* cExpr = static_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);

    std::vector<const Symbol*> overloadCandidates = mSymbolTable->GetOverloadCandidates(cExpr->GetCalleeName());
    
    const ChildrenNodes& argNodes = cExpr->GetArgs();
    const size_t argSize = argNodes.size();

    // Remove the candidates with a different number of arguments
    overloadCandidates.erase(std::remove_if(overloadCandidates.begin(), overloadCandidates.end(), 
                                            [&argSize](const Symbol* fnSym) 
                                            { 
                                                return fnSym->GetFunctionParamTypes().size() != argSize; 
                                            }), overloadCandidates.end());


    // No matches with the given number of arguments
    if (overloadCandidates.empty())
    {
        ++mErrorCount;
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_NB_ARGS_ERROR, mCurrentNode->GetSourceLocation());
        mOverloadMap[cExpr] = {};
        return;
    }

    // Progressively eliminate candidates whose arguments types don't match from the overload set
    for (size_t iArg = 0; iArg < argSize; ++iArg)
    {
        const std::unique_ptr<ASTNode>& node = argNodes[iArg];

        if (node->GetKind() != ASTNode::NodeKind::CALL_EXPR)
        {
            overloadCandidates.erase(std::remove_if(overloadCandidates.begin(), overloadCandidates.end(), 
                                                    [&iArg, &node, this](const Symbol* fnSym) 
                                                    {
                                                        return fnSym->GetFunctionParamTypeAt(iArg) != mNodeTypes[node.get()];
                                                    }), overloadCandidates.end());
        }
        else
        {
            const std::vector<const Symbol*> overloadSet = mOverloadMap[node.get()];

            // Bail out if the previous call wasn't resolved 
            if (overloadSet.empty())
                return;
            
            // Enumerate the possible types of the argument
            std::vector<Common::Type> possibleTypes;
            std::transform(overloadSet.begin(), overloadSet.end(), std::back_inserter(possibleTypes),
                           [](const Symbol* sym) { return sym->GetFunctionReturnType(); });

            // Remove duplicates
            std::sort(possibleTypes.begin(), possibleTypes.end());
            possibleTypes.erase(std::unique(possibleTypes.begin(), possibleTypes.end()), possibleTypes.end());

            // Remove candidates that don't require one of the possible types at iArg index
            overloadCandidates.erase(std::remove_if(overloadCandidates.begin(), overloadCandidates.end(),
                                     [&possibleTypes](const Symbol* fnSym)
                                     {
                                         const Common::Type fnRType = fnSym->GetFunctionReturnType();

                                         return std::none_of(possibleTypes.begin(), possibleTypes.end(), 
                                                             [&fnRType](const Common::Type ty) { return fnRType == ty; });
                                     }), overloadCandidates.end());
        }
    }

    // This handler doesn't really perform type checking on a call expression. 
    // Instead, it makes sure that the call has a valid overload set which will 
    // later be resolved when type checking the node making use of this call.
    // The fact is that without context we can't perform a full type check on a function call.
    if (overloadCandidates.empty())
    {
        ++mErrorCount;
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_PARAM_TYPE_ERROR, mCurrentNode->GetSourceLocation());
        mOverloadMap[cExpr] = {};
        return;
    }

    mOverloadMap[cExpr] = overloadCandidates;
}

void TypeChecker::HandleIdentifierExpr()
{
    // Performs no type checking. This method is for associating a type to an identifier

    IdentifierExpr* iExpr = static_cast<IdentifierExpr*>(this->mCurrentNode);
    assert(iExpr != nullptr);

    const Symbol* varSym;
    bool symFound;
    std::tie(symFound, varSym) = mSymbolTable->TryGetSymbol(iExpr);

    // It is assumed that the symbol has been declared. If not, it's because 
    // this pass is being run without having run the symbol collector properly.
    assert(symFound);

    mNodeTypes[iExpr] = varSym->GetVariableType();
    iExpr->SetType(varSym->GetVariableType());
}

void TypeChecker::HandleIndexedExpr()
{
    const IndexedExpr* iExpr = static_cast<const IndexedExpr*>(this->mCurrentNode);
    assert(iExpr != nullptr);

    // The index used in the expression must evaluate to a number
    if (!CheckExprEvaluateToType(iExpr->GetIndex(), Type::NUMBER))
    {
        // TODO: Log an error and test it
        ++mErrorCount;
    }

    // If the expression is correct, we associate it with the type of its identifier
    mNodeTypes[iExpr] = mNodeTypes.at(iExpr->GetIdentifier());
}


void TypeChecker::HandleNumberExpr()
{
    mNodeTypes[this->mCurrentNode] = Type::NUMBER;
}

void TypeChecker::HandleStringExpr()
{
    mNodeTypes[this->mCurrentNode] = Type::STRING;
}

void TypeChecker::HandleSpawnExpr()
{
    const SpawnExpr* sExpr = static_cast<const SpawnExpr*>(this->mCurrentNode);
    assert(sExpr != nullptr);

    // A spawn can only be put before a function call
    if (sExpr->GetChildrenNodes().front()->GetKind() != ASTNode::NodeKind::CALL_EXPR)
    {
        ++mErrorCount;
    }
}

void TypeChecker::HandleIfStmt()
{
    const IfStmt* ifStmt = static_cast<const IfStmt*>(this->mCurrentNode);
    assert(ifStmt != nullptr);

    if (!CheckExprEvaluateToType(ifStmt->GetCondExpr(), Type::BOOL))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE, ifStmt->GetCondExpr()->GetSourceLocation());
        ++mErrorCount;
    }
}

void TosLang::FrontEnd::TypeChecker::HandleReturnStmt()
{
    const ReturnStmt* rStmt = static_cast<const ReturnStmt*>(this->mCurrentNode);
    assert(rStmt != nullptr);

    const Symbol* fnSymbol;
    bool symFound;
    std::tie(symFound, fnSymbol) = mSymbolTable->TryGetSymbol(mCurrentFunc);
    assert(symFound);

    Type fnType = fnSymbol->GetFunctionReturnType();
    if (fnType == Type::VOID)
    {
        if(rStmt->GetReturnExpr() != nullptr)
        { 
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_RETURN_TYPE, rStmt->GetSourceLocation());
            ++mErrorCount;
        }
    }
    else
    {
        if (!CheckExprEvaluateToType(rStmt->GetReturnExpr(), fnType))
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_RETURN_TYPE, rStmt->GetSourceLocation());
            ++mErrorCount;
        }
    }
}

void TypeChecker::HandleSleepStmt()
{
    const SleepStmt* sStmt = static_cast<const SleepStmt*>(this->mCurrentNode);
    assert(sStmt != nullptr);

    if (!CheckExprEvaluateToType(sStmt->GetCountExpr(), Type::NUMBER))
    {
        // TODO: Log an error and test it
        ++mErrorCount;
    }
}

void TypeChecker::HandleWhileStmt()
{
    const WhileStmt* wStmt = static_cast<const WhileStmt*>(this->mCurrentNode);
    assert(wStmt != nullptr);

    if (!CheckExprEvaluateToType(wStmt->GetCondExpr(), Type::BOOL))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE, wStmt->GetCondExpr()->GetSourceLocation());
        ++mErrorCount;
    }
}
