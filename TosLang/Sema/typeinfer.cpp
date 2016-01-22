#include "typeinfer.h"

#include "../AST/declarations.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

TosLang::FrontEnd::TypeInfer::TypeInfer(const std::shared_ptr<SymbolTable>& symTab) : mSymbolTable{ symTab }
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

void TosLang::FrontEnd::TypeInfer::Infer(const std::unique_ptr<ASTNode>& /*root*/)
{
}

void TosLang::FrontEnd::TypeInfer::HandleFunctionDecl()
{
}

void TosLang::FrontEnd::TypeInfer::HandleVarDecl()
{
    const VarDecl* varDecl = dynamic_cast<const VarDecl*>(mCurrentNode);
    assert(varDecl != nullptr);

    const Expr* initExpr = varDecl->GetInitExpr();
    if (initExpr != nullptr)
    {
        Type varType = Type::UNKNOWN;

        switch (initExpr->GetKind())
        {
        case ASTNode::NodeKind::BOOLEAN_EXPR:
            varType = Type::BOOL;
            break;
        case ASTNode::NodeKind::IDENTIFIER_EXPR:
            varType = Type::BOOL;
            break;
        case ASTNode::NodeKind::NUMBER_EXPR:
            varType = Type::NUMBER;
            break;
        default:
            // TODO: Log an error
            break;
        }

        //mSymbolTable->AddSymbol(varDecl->GetName(), { varType, mCurrentScopeLevel });
    }
    else
    {
        // TODO: Log an error
        return;
    }

    
}
