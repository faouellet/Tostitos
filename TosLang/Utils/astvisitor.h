#ifndef AST_VISITOR_H__TOSTITOS
#define AST_VISITOR_H__TOSTITOS

#include "../AST/ast.h"

namespace TosLang
{
    namespace Utils
    {
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
            /*
            * \fn VisitPostOrder
            * \param root   Root node of the tree to visit
            * \brief        Visit the tree rooted at root by doing a post-order traversal
            */
            void VisitPostOrder(const std::unique_ptr<FrontEnd::ASTNode>& root)
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

            /*
            * \fn VisitPreOrder
            * \param root   Root node of the tree to visit
            * \brief        Visit the tree rooted at root by doing a pre-order traversal
            */
            void VisitPreOrder(const std::unique_ptr<FrontEnd::ASTNode>& root)
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

        protected:  // Declarations
            /*
            * \fn       HandleFunctionDecl
            * \brief    Handle a node of the FUNCTION_DECL kind
            */
            void HandleFunctionDecl() { }

            /*
            * \fn       HandleParamVarDecl
            * \brief    Handle a node of the PARAM_VAR_DECL kind
            */
            void HandleParamVarDecl() { }

            /*
            * \fn       HandleProgramDecl
            * \brief    Handle a node of the PROGRAM_DECL kind
            */
            void HandleProgramDecl() { }

            /*
            * \fn       HandleVarDecl
            * \brief    Handle a node of the VAR_DECL kind
            */
            void HandleVarDecl() { }

        protected:  // Expressions
            /*
            * \fn       HandleBinrayExpr
            * \brief    Handle a node of the BINARY_EXPR kind
            */
            void HandleBinaryExpr() { }

            /*
            * \fn       HandleBooleanExpr
            * \brief    Handle a node of the BOOLEAN_EXPR kind
            */
            void HandleBooleanExpr() { }

            /*
            * \fn       HandleCallExpr
            * \brief    Handle a node of the CALL_EXPR kind
            */
            void HandleCallExpr() { }

            /*
            * \fn       HandleIdentifierExpr
            * \brief    Handle a node of the IDENTIFIER_EXPR kind
            */
            void HandleIdentifierExpr() { }

            /*
            * \fn       HandleNumberExpr
            * \brief    Handle a node of the NUMBER_EXPR kind
            */
            void HandleNumberExpr() { }
        
        protected:  // Statements
            /*
            * \fn       HandleCompoundStmt
            * \brief    Handle a node of the COMPOUND_STMT kind
            */
            void HandleCompoundStmt() { }

            /*
            * \fn       HandleIfStmt
            * \brief    Handle a node of the IF_STMT kind
            */
            void HandleIfStmt() { }

            /*
            * \fn       HandlePrintStmt
            * \brief    Handle a node of the PRINT_STMT kind
            */
            void HandlePrintStmt() { }

            /*
            * \fn       HandleReturnStmt
            * \brief    Handle a node of the RETURN_STMT kind
            */
            void HandleReturnStmt() { }

            /*
            * \fn       HandleScanStmt
            * \brief    Handle a node of the SCAN_STMT kind
            */
            void HandleScanStmt() { }

            /*
            * \fn       HandleWhileStmt
            * \brief    Handle a node of the WHILE_STMT kind
            */
            void HandleWhileStmt() { }

        private:
            /*
            * \fn       GetDerived
            * \brief    Gets a reference to the current derived class of ASTVisitor
            * \return   Reference to the current derived class of ASTVisitor
            */
            Derived& GetDerived()
            {
                return *static_cast<Derived*>(this);
            }

            /*
            * \fn           DispatchNode
            * \param node   AST node to dispatch
            * \brief        Dispatch the handling of given node to the correct method of the correct derived class
            */
            void DispatchNode(const FrontEnd::ASTNode* node)
            {
                switch (node->GetKind())
                {
                    ////// Declarations //////
                case FrontEnd::ASTNode::NodeKind::FUNCTION_DECL:
                    GetDerived().HandleFunctionDecl();
                    break;
                case FrontEnd::ASTNode::NodeKind::PARAM_VAR_DECL:
                    GetDerived().HandleParamVarDecl();
                    break;
                case FrontEnd::ASTNode::NodeKind::PROGRAM_DECL:
                    GetDerived().HandleProgramDecl();
                    break;
                case FrontEnd::ASTNode::NodeKind::VAR_DECL:
                    GetDerived().HandleVarDecl();
                    break;
                    ////// Expressions //////
                case FrontEnd::ASTNode::NodeKind::BINARY_EXPR:
                    GetDerived().HandleBinaryExpr();
                    break;
                case FrontEnd::ASTNode::NodeKind::BOOLEAN_EXPR:
                    GetDerived().HandleBooleanExpr();
                    break;
                case FrontEnd::ASTNode::NodeKind::CALL_EXPR:
                    GetDerived().HandleCallExpr();
                    break;
                case FrontEnd::ASTNode::NodeKind::IDENTIFIER_EXPR:
                    GetDerived().HandleIdentifierExpr();
                    break;
                case FrontEnd::ASTNode::NodeKind::NUMBER_EXPR:
                    GetDerived().HandleNumberExpr();
                    break;
                    ////// Statements //////
                case FrontEnd::ASTNode::NodeKind::COMPOUND_STMT:
                    GetDerived().HandleCompoundStmt();
                    break;
                case FrontEnd::ASTNode::NodeKind::IF_STMT:
                    GetDerived().HandleIfStmt();
                    break;
                case FrontEnd::ASTNode::NodeKind::PRINT_STMT:
                    GetDerived().HandlePrintStmt();
                    break;
                case FrontEnd::ASTNode::NodeKind::RETURN_STMT:
                    GetDerived().HandleReturnStmt();
                    break;
                case FrontEnd::ASTNode::NodeKind::SCAN_STMT:
                    GetDerived().HandleScanStmt();
                    break;
                case FrontEnd::ASTNode::NodeKind::WHILE_STMT:
                    GetDerived().HandleWhileStmt();
                    break;
                default:
                    break;
                }
            }

        protected:
            unsigned int mCurrentLevel; /*!< Current tree level of the traversal */
            FrontEnd::ASTNode* mCurrentNode;      /*!< Current node being handled in the traversal */
        };
    }
}

#endif // AST_VISITOR_H__TOSTITOS