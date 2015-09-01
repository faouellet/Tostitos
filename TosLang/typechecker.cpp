#include "typechecker.h"

#include "declarations.h"
#include "errorlogger.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

// Declarations

unsigned TypeChecker::TypeCheck(const std::unique_ptr<ASTNode>& root)
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
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr().get());
    if (initExpr != nullptr)
    {
        Symbol initSymbol;
        Symbol varSymbol;

        if (!mSymbolTable->GetSymbol(vDecl->GetName(), varSymbol) 
            || !mSymbolTable->GetSymbol(initExpr->GetName(), initSymbol))
        {
            // TODO: Should this be in another semantic analysis pass?
            ErrorLogger::PrintError(ErrorLogger::VAR_UNDECLARED_IDENTIFIER);
            ++mErrorCount;
        }
        else if ((initExpr->GetKind() == ASTNode::BOOLEAN_EXPR || initExpr->GetKind() == ASTNode::NUMBER_EXPR) 
            && varSymbol.mType != initSymbol.mType) 
        {
            ErrorLogger::PrintError(ErrorLogger::WRONG_LITERAL_TYPE);
            ++mErrorCount;
        }
        else
        {
            Symbol varSymbol;
            // TODO: Generalize to include the case when we try to assign to an undeclared variable
            mSymbolTable->GetSymbol(initExpr->GetName(), initSymbol);
            if (varSymbol.mType != initSymbol.mType)
            {
                ErrorLogger::PrintError(ErrorLogger::WRONG_VARIABLE_TYPE);
                ++mErrorCount;
            }
        }
    }
}
