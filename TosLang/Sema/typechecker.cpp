#include "typechecker.h"

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

bool TosLang::FrontEnd::TypeChecker::TryGetSymbol(const std::string& fnName, const std::string& symName, const std::stack<int>& scopesToSearch, Symbol& sym)
{
    if (!mSymbolTable->GetSymbol(fnName, symName, scopesToSearch, sym))
    {
        // Variable wasn't declared
        ErrorLogger::PrintError(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER);
        ++mErrorCount;
        return false;
    }
    return true;
}

void TosLang::FrontEnd::TypeChecker::CheckCondExprEvaluateToBool(const Expr* condExpr)
{
    switch (condExpr->GetKind())
    {
    case ASTNode::NodeKind::BOOLEAN_EXPR:
        // Trivial, nothing to do
        break;
    case ASTNode::NodeKind::BINARY_EXPR:
    {
        const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(condExpr);
        if (mBinOpTypes[bExpr] != Type::BOOL)
        {
            ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE);
            ++mErrorCount;
        }
    }
    break;
    case ASTNode::NodeKind::CALL_EXPR:
    {
        const CallExpr* cExpr = dynamic_cast<const CallExpr*>(condExpr);
        Symbol callSym;
        if (!TryGetSymbol(mCurrentFunc->GetName(), cExpr->GetName(), mCurrentScopesTraversed, callSym))
        {
            ErrorLogger::PrintError(ErrorLogger::ErrorType::FN_UNDECLARED);
            ++mErrorCount;
        }

        if (callSym.GetFunctionReturnType() != Type::BOOL)
        {
            ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE);
            ++mErrorCount;
        }
    }
    break;
    default:    // This will happens when the condition expression is a string or a number
        ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_COND_EXPR_TYPE);
        ++mErrorCount;
        break;
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
        Symbol varSymbol;
        // Get the name of the function currently traversed. An empty name means that we're not in a function, we're in the global scope
        std::string fnName = mCurrentFunc == nullptr ? "" : mCurrentFunc->GetName();
        if (!TryGetSymbol(fnName, vDecl->GetName(), mCurrentScopesTraversed, varSymbol))
            return;

        // Check of there is a mismatch between the variable type and the type of its initialization expression
        // (for boolean or number literals)
        if ((initExpr->GetKind() == ASTNode::NodeKind::BOOLEAN_EXPR && varSymbol.GetVariableType() != Type::BOOL)
            || (initExpr->GetKind() == ASTNode::NodeKind::NUMBER_EXPR && varSymbol.GetVariableType() != Type::NUMBER))
        {
            ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_LITERAL_TYPE);
            ++mErrorCount;
            return;
        }
        
        const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(initExpr);
        if (bExpr != nullptr)
        {
            // Check of there is a mismatch between the variable type and the type of its initialization expression
            // (when the initialization expression is a binary expression)
            if (varSymbol.GetVariableType() != mBinOpTypes[bExpr])
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_EXPR_TYPE);
                ++mErrorCount;
            }
        }
        else
        {
            // Check of there is a mismatch between the variable type and the type of its initialization expression
            // (when the initialization expression is a another variable or a function call)
            Symbol initSymbol;
            if (TryGetSymbol(fnName, initExpr->GetName(), mCurrentScopesTraversed, initSymbol))
            {
                if (varSymbol.GetVariableType() != initSymbol.GetFunctionReturnType())
                {
                    ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_VARIABLE_TYPE);
                    ++mErrorCount;
                }
            }
        }
    }
}

void TosLang::FrontEnd::TypeChecker::HandleBinaryExpr()
{
    const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(this->mCurrentNode);
    assert(bExpr != nullptr);

    const ChildrenNodes& children = bExpr->GetChildrenNodes();
    Type operandTypes[2];
    for (int i = 0; i < 2; ++i)
    {
        switch (children[i]->GetKind())
        {
        case ASTNode::NodeKind::BINARY_EXPR:
        {
            const BinaryOpExpr* innerBExpr = dynamic_cast<const BinaryOpExpr*>(children[i].get());
            operandTypes[i] = mBinOpTypes[innerBExpr];
        }
            break;
        case ASTNode::NodeKind::BOOLEAN_EXPR:
            operandTypes[i] = Type::BOOL;
            break;
        case ASTNode::NodeKind::CALL_EXPR:
        {
            const CallExpr* cExpr = dynamic_cast<const CallExpr*>(children[i].get());
            Symbol callSym;
            if (!TryGetSymbol(mCurrentFunc->GetName(), cExpr->GetName(), mCurrentScopesTraversed, callSym))
            {
                // Trying to call a function that doesn't exist
                mBinOpTypes[bExpr] = Type::ERROR;
                return;
            }
            operandTypes[i] = callSym.GetFunctionReturnType();
        }
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
            // It is an error to use binary expression with an identifier as one of its operands
            mBinOpTypes[bExpr] = Type::ERROR;
            return;
        case ASTNode::NodeKind::NUMBER_EXPR:
            operandTypes[i] = Type::NUMBER;
            break;
        }
    }

    // Check if the operands' types match
    if (operandTypes[0] != operandTypes[1])
    {
        ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_BIN_EXPR_TYPE);
        ++mErrorCount;
        return;
    }

    mBinOpTypes[bExpr] = operandTypes[0];
}

void TosLang::FrontEnd::TypeChecker::HandleCallExpr()
{
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);

    Symbol fnSymbol;
    if (!mSymbolTable->GetSymbol("", cExpr->GetCalleeName(), mCurrentScopesTraversed, fnSymbol))
    {
        ErrorLogger::PrintError(ErrorLogger::ErrorType::FN_UNDECLARED);
        ++mErrorCount;
        return;
    }

    std::vector<Type> expectedArgTypes = fnSymbol.GetFunctionParamTypes();
    const std::vector<std::unique_ptr<ASTNode>>& args = cExpr->GetArgs();

    if (expectedArgTypes.size() != args.size())
    {
        ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_NB);
        ++mErrorCount;
        return;
    }

    for (size_t i = 0; i < args.size(); i++)
    {
        auto& arg = args[i];
        Type expectedArgTy = expectedArgTypes[i];

        // TODO: Too much repetition down below
        switch (arg->GetKind())
        {
        case ASTNode::NodeKind::BINARY_EXPR:
        {
            const BinaryOpExpr* bExpr = dynamic_cast<const BinaryOpExpr*>(arg.get());
            if (expectedArgTy != mBinOpTypes[bExpr])
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_TYPE);
                ++mErrorCount;
                return;
            }
        }
            break;
        case ASTNode::NodeKind::BOOLEAN_EXPR:
            if (expectedArgTy != Type::BOOL)
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_TYPE);
                ++mErrorCount;
                return;
            }
            break;
        case ASTNode::NodeKind::CALL_EXPR:
        {
            Symbol fnSym;
            if (!TryGetSymbol(mCurrentFunc->GetName(), arg->GetName(), mCurrentScopesTraversed, fnSym))
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::FN_UNDECLARED);
                ++mErrorCount;
                return;
            }

            if (expectedArgTy != fnSym.GetFunctionReturnType())
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_TYPE);
                ++mErrorCount;
                return;
            }
        }
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
        {
            Symbol varSym;
            if (!TryGetSymbol(mCurrentFunc->GetName(), arg->GetName(), mCurrentScopesTraversed, varSym))
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER);
                ++mErrorCount;
                return;
            }

            if (expectedArgTy != varSym.GetVariableType())
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_TYPE);
                ++mErrorCount;
                return;
            }
        }
            break;
        case ASTNode::NodeKind::NUMBER_EXPR:
            if (expectedArgTy != Type::NUMBER)
            {
                ErrorLogger::PrintError(ErrorLogger::ErrorType::CALL_WRONG_ARG_TYPE);
                ++mErrorCount;
                return;
            }
            break;
        default:
            // This shouldn't be possible
            assert(false);
            break;
        }
    }
}

void TosLang::FrontEnd::TypeChecker::HandleIfStmt()
{
    const IfStmt* ifStmt = dynamic_cast<const IfStmt*>(this->mCurrentNode);
    assert(ifStmt != nullptr);

    CheckCondExprEvaluateToBool(ifStmt->GetCondExpr());

}

void TosLang::FrontEnd::TypeChecker::HandlePrintStmt()
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
            ErrorLogger::PrintError(ErrorLogger::ErrorType::PRINT_WRONG_INPUT_TYPE);
            ++mErrorCount;
        }
    }
}

void TosLang::FrontEnd::TypeChecker::HandleScanStmt()
{
    const ScanStmt* sStmt = dynamic_cast<const ScanStmt*>(this->mCurrentNode);
    assert(sStmt != nullptr);

    const IdentifierExpr* iExpr = sStmt->GetInput();
    if (iExpr == nullptr)
    {
        // Expression is not an identifier, log an error
        ErrorLogger::PrintError(ErrorLogger::ErrorType::SCAN_WRONG_INPUT_TYPE);
        ++mErrorCount;
    }
}

void TosLang::FrontEnd::TypeChecker::HandleWhileStmt()
{
    const WhileStmt* wStmt = dynamic_cast<const WhileStmt*>(this->mCurrentNode);
    assert(wStmt != nullptr);

    CheckCondExprEvaluateToBool(wStmt->GetCondExpr());
}
