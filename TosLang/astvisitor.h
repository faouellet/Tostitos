#ifndef AST_VISTOR_H__TOSTITOS
#define AST_VISITOR_H__TOSTITOS

#include "ast.h"

/*
* \class ASTVisitor
* \brief Tree visitor that does a post-order traversal of the AST.
*        It is based of CRTP, so any derived class should provide itself as the template argument
*/
template <typename Derived>
class ASTVisitor
{
public:
    void Visit(const std::unique_ptr<ASTNode>& root)
    {
        if (root)
        {
            for (auto& childNode : root->GetChildrenNodes())
                Visit(childNode);
            DispatchNode(root.get());
        }
    }

    // Declarations
    void HandleProgramDecl() { }
    void HandleVarDecl() { }

    // Expressions
    void HandleBooleanExpr() { }
    void HandleNumberExpr() { }

private:
    Derived& GetDerived()
    {
        return static_cast<Derived*>(this);
    }

    void DispatchNode(const ASTNode* node)
    {
        switch (node->GetKind())
        {
        case ASTNode::PROGRAM_DECL:
            GetDerived().HandleProgramDecl();
            break;
        case ASTNode::VAR_DECL:
            GetDerived().HandleVarDecl();
            break;
        case ASTNode::BOOLEAN_EXPR:
            GetDerived().HandleBooleanExpr();
            break;
        case ASTNode::NUMBER_EXPR:
            GetDerived().HandleNumberExpr();
            break;
        default:
            break;
        }
    }
};

#endif // AST_VISITOR_H__TOSTITOS