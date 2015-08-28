#include "typechecker.h"

#include "declarations.h"
#include "errorlogger.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

// Declarations

void TypeChecker::TypeCheck(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
}

void TypeChecker::HandleVarDecl()
{
    const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);

    // Add the variable to the type table
    mTypeTable[vDecl->GetVarName()] = vDecl->GetType();
    
    // We only perform a type check when a variable is declared AND initialized at the same time
    const Expr* initExpr = dynamic_cast<const Expr*>(vDecl->GetInitExpr().get());
    if (initExpr != nullptr)
    {
        // TODO: How come IsLiteral doesn't work here?
        if (initExpr->GetKind() == ASTNode::BOOLEAN_EXPR || initExpr->GetKind() == ASTNode::NUMBER_EXPR) // Either Int or Bool
        {
            if (initExpr->GetKind() != vDecl->GetType())
                ErrorLogger::PrintError(ErrorLogger::WRONG_LITERAL_TYPE);

        }
        else
        {
            // TODO
        }
    }
}
