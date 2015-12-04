#include "typechecker.h"

#include "../AST/declarations.h"
#include "../Utils/errorlogger.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
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
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr());
    if (initExpr != nullptr)
    {
        Symbol varSymbol;
        if (!mSymbolTable->GetSymbol(vDecl->GetName(), varSymbol))
        {
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

            if (!mSymbolTable->GetSymbol(initExpr->GetName(), initSymbol))
            {
                // TODO: Should this be in another semantic analysis pass?
                ErrorLogger::PrintError(ErrorLogger::ErrorType::VAR_UNDECLARED_IDENTIFIER);
                ++mErrorCount;
            }
        }
    }
}
