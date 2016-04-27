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

size_t TypeChecker::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    mErrorCount = 0;
    mSymbolTable = symTab;
    mNodeTypes.clear();
    mOverloadMap.clear();
    this->VisitPostOrder(root);
    return mErrorCount;
}

bool TypeChecker::CheckExprEvaluateToType(const Expr* expr, Type type)
{
    switch (expr->GetKind())
    {
    case ASTNode::NodeKind::BOOLEAN_EXPR:
    {
        mNodeTypes[expr] = Type::BOOL;
        return type == Type::BOOL;
    }
    case ASTNode::NodeKind::BINARY_EXPR:
    {
        return mNodeTypes[expr] == type;
    }
    case ASTNode::NodeKind::CALL_EXPR:
    {
        const CallExpr* cExpr = dynamic_cast<const CallExpr*>(expr);
        assert(cExpr != nullptr);
     
        // The overload set contains all functions that have matching parameters.
        // Now, we need to complete the overload resolution of the function call 
        // by finding a function that also has a matching return type.
        const std::vector<const Symbol*>& overloadSet = mOverloadMap.at(cExpr);
        auto matchIt = std::find_if(overloadSet.begin(), overloadSet.end(), 
                                    [&type](const Symbol* sym) 
                                    { 
                                        return sym->GetFunctionReturnType() == type; 
                                    });

        if (matchIt != overloadSet.end())
        {
            mSymbolTable->AddFunctionUse(cExpr, **matchIt);
            return true;
        }
        else
        {
            return false;
        }
    }
    case ASTNode::NodeKind::IDENTIFIER_EXPR:
    {
        return type == mNodeTypes[expr];
    }
    case ASTNode::NodeKind::NUMBER_EXPR:
    {
        mNodeTypes[expr] = Type::NUMBER;
        return type == Type::NUMBER;
    }
    case ASTNode::NodeKind::STRING_EXPR:
    {
        mNodeTypes[expr] = Type::STRING;
        return type == Type::STRING;
    }
    default:    
        // This shouldn't happen
        assert(false);
        return false;
    }
}

void TypeChecker::HandleVarDecl()
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
    assert(vDecl != nullptr);
        
    // We only perform a type check when a variable is declared AND initialized at the same time
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr());
    if (initExpr != nullptr)
    {
        const Symbol* varSymbol;
        bool symFound;
        std::tie(symFound, varSymbol) = mSymbolTable->TryGetSymbol(vDecl);

        assert(symFound);

        if (!CheckExprEvaluateToType(initExpr, varSymbol->GetVariableType()))
        {
            switch (initExpr->GetKind())
            {
            case ASTNode::NodeKind::BOOLEAN_EXPR:
            case ASTNode::NodeKind::NUMBER_EXPR:
            case ASTNode::NodeKind::STRING_EXPR:
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_LITERAL_TYPE, initExpr->GetSourceLocation());
                ++mErrorCount;
                return;
            case ASTNode::NodeKind::BINARY_EXPR:
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_EXPR_TYPE, initExpr->GetSourceLocation());
                ++mErrorCount;
                return;
            case ASTNode::NodeKind::CALL_EXPR:
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_CALL_TYPE, mCurrentNode->GetSourceLocation());
                ++mErrorCount;
                return;
            case ASTNode::NodeKind::IDENTIFIER_EXPR:
                ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_VARIABLE_TYPE, mCurrentNode->GetSourceLocation());
                ++mErrorCount;
                return;
            default:
                // This shouldn't happen
                assert(false);
            }
        }
    }
}

void TypeChecker::HandleBinaryExpr()
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(this->mCurrentNode);
    assert(bExpr != nullptr);

    const ChildrenNodes& children = bExpr->GetChildrenNodes();
    assert(children.size() == 2);
    Type operandTypes[2];
    for (int i = 0; i < 2; ++i)
    {
        switch (children[i]->GetKind())
        {
        case ASTNode::NodeKind::BINARY_EXPR:
        {
            const BinaryOpExpr* innerBExpr = dynamic_cast<const BinaryOpExpr*>(children[i].get());
            operandTypes[i] = mNodeTypes.at(innerBExpr);
        }
            break;
        case ASTNode::NodeKind::BOOLEAN_EXPR:
            operandTypes[i] = Type::BOOL;
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
        {
            const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(children[i].get());
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

    if ((bExpr->GetOperation() == Operation::GREATER_THAN) || (bExpr->GetOperation() == Operation::LESS_THAN))
        // When using either '<' or '>', the resulting type will be different than the operand types.
        // It is using number expressions to produce a boolean expression.
        mNodeTypes[bExpr] = Type::BOOL;
    else
        mNodeTypes[bExpr] = operandTypes[0];
}

void TypeChecker::HandleCallExpr()
{
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);

    // Get the arguments' types for the function being called
    std::vector<Common::Type> argTypes;
    std::transform(cExpr->GetArgs().begin(), cExpr->GetArgs().end(), std::back_inserter(argTypes),
                   [this](const std::unique_ptr<ASTNode>& node)
                   {
                       if (node->GetKind() == ASTNode::NodeKind::CALL_EXPR)
                       {

                       }
                       return mNodeTypes.at(node.get());
                   });
    
    const std::vector<const Symbol*> overloadSet = mSymbolTable->GetOverloadSet(cExpr->GetCalleeName(), argTypes);

    // This handler doesn't really perform type checking on a call expression. 
    // Instead, it makes sure that the call has a valid overload set which will 
    // later be resolved when type checking the node making use of this call.
    // The fact is that without context we can't perform a type check on a function call.
    if (overloadSet.empty())
    {
        ++mErrorCount;
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_NO_OVERLOAD, mCurrentNode->GetSourceLocation());
        return;
    }

    mOverloadMap[cExpr] = overloadSet;
}

void TypeChecker::HandleIdentifierExpr()
{
    // Performs no type checking. This method is for associating a type to an identifier

    const IdentifierExpr* iExpr = dynamic_cast<const IdentifierExpr*>(this->mCurrentNode);
    assert(iExpr != nullptr);

    const Symbol* varSym;
    bool symFound;
    std::tie(symFound, varSym) = mSymbolTable->TryGetSymbol(iExpr);

    // It is assumed that the symbol has been declared. If not, it's because 
    // this pass is being run without having run the symbol collector properly.
    assert(symFound);

    mNodeTypes[iExpr] = varSym->GetVariableType();
}

void TypeChecker::HandleIfStmt()
{
    const IfStmt* ifStmt = dynamic_cast<const IfStmt*>(this->mCurrentNode);
    assert(ifStmt != nullptr);

    if (!CheckExprEvaluateToType(ifStmt->GetCondExpr(), Type::BOOL))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE, ifStmt->GetCondExpr()->GetSourceLocation());
        ++mErrorCount;
    }
}

void TypeChecker::HandlePrintStmt()
{
    const PrintStmt* pStmt = dynamic_cast<const PrintStmt*>(this->mCurrentNode);
    assert(pStmt != nullptr);

    const Expr* msgExpr = pStmt->GetMessage();
    if (msgExpr != nullptr)
    {
        if (msgExpr->GetKind() != ASTNode::NodeKind::IDENTIFIER_EXPR 
            && msgExpr->GetKind() != ASTNode::NodeKind::NUMBER_EXPR)
        {
            // Expression is not an identifier or a number, log an error
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PRINT_WRONG_INPUT_TYPE, pStmt->GetSourceLocation());
            ++mErrorCount;
        }
    }
}

void TosLang::FrontEnd::TypeChecker::HandleReturnStmt()
{
    const ReturnStmt* rStmt = dynamic_cast<const ReturnStmt*>(this->mCurrentNode);
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

void TypeChecker::HandleWhileStmt()
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(this->mCurrentNode);
    assert(wStmt != nullptr);

    if (!CheckExprEvaluateToType(wStmt->GetCondExpr(), Type::BOOL))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE, wStmt->GetCondExpr()->GetSourceLocation());
        ++mErrorCount;
    }
}
