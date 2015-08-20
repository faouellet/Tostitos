#ifndef AST_VISTOR_H__TOSTITOS
#define AST_VISITOR_H__TOSTITOS

#include "ast.h"

/*
* \class ASTVisitor
* \brief Tree visitor that does a traversal of the AST.
*        It is based of CRTP, so any derived class should provide itself as the template argument
*/
template <typename Derived>
class ASTVisitor
{
public:
    ASTVisitor() : mCurrentLevel(0), mCurrentNode(nullptr) { }
    ~ASTVisitor() = default;

protected:
    void VisitPostOrder(const std::unique_ptr<ASTNode>& root)
    {
        if (root != nullptr)
        {
            ++mCurrentLevel;

            for (auto& childNode : root->GetChildrenNodes())
                VisitPostOrder(childNode);

            --mCurrentLevel;
            mCurrentNode = root.get();
            
            DispatchNode(root.get());
        }
    }

    void VisitPreOrder(const std::unique_ptr<ASTNode>& root)
    {
        if (root != nullptr)
        {
            mCurrentNode = root.get();
            DispatchNode(root.get());

            ++mCurrentLevel;

            for (auto& childNode : root->GetChildrenNodes())
                VisitPreOrder(childNode);

            --mCurrentLevel;
        }
    }

public:
    // Declarations
    void HandleProgramDecl() { }
    void HandleVarDecl() { }

    // Expressions
    void HandleBooleanExpr() { }
    void HandleNumberExpr() { }

private:
    Derived& GetDerived()
    {
        return *static_cast<Derived*>(this);
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

protected:
    unsigned int mCurrentLevel;
    ASTNode* mCurrentNode;
};

#endif // AST_VISITOR_H__TOSTITOS