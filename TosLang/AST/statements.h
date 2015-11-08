#ifndef STMT_H__TOSTITOS
#define STMT_H__TOSTITOS

#include "ast.h"
#include "expressions.h"

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class Stmt
        * \brief Node of the AST representing a statement
        */
        class Stmt : public ASTNode
        {
        public:
            explicit Stmt(NodeKind kind) : ASTNode{ kind } { }
            virtual ~Stmt() { }
        };

        /*
        * \class CompoundStmt
        * \brief Node of the AST representing a block of statement. 
        *        In more practical terms, it corresponds to a list of statements within a given scope.
        */
        class CompoundStmt : public Stmt
        {
        public:
            CompoundStmt() : Stmt{ NodeKind::COMPOUND_STMT } {}
            virtual ~CompoundStmt() { }

        public:
            void AddStatement(std::unique_ptr<ASTNode>&& stmt) { AddChildNode(std::move(stmt)); }
            const ChildrenNodes& GetStatements() const { return mChildren; }
        };

        /*
        * \class IfStmt
        * \brief Node of the AST representing a IF
        */
        class IfStmt : public Stmt
        {
        public:
            IfStmt(std::unique_ptr<Expr>&& condExpr, std::unique_ptr<CompoundStmt>&& thenStmt) :
                Stmt{ NodeKind::IF_STMT }
            {
                assert(condExpr != nullptr);
                assert(thenStmt != nullptr);

                AddChildNode(std::move(condExpr));
                AddChildNode(std::move(thenStmt));
            }

            virtual ~IfStmt() { }

        public:
            /*
            * \fn       GetCondExpr
            * \brief    Gets the conditional expression evaluated in the if node
            * \return   Conditional expression of the if node
            */
            const Expr* GetCondExpr() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(0); }

            /*
            * \fn       GetThenStmt
            * \brief    Gets the actions performed by the if node when its condition evaluates to true
            * \return   Statements representing the actions to be performed by the if node when it takes the then branch
            */
            const CompoundStmt* GetThenExpr() const { assert(mChildren.size() == 2); return GetChildNodeAs<CompoundStmt>(1); }
        };

        /*
        * \class PrintStmt
        * \brief Node of the AST representing a PRINTLN
        */
        class PrintStmt : public Stmt
        {
        public:
            PrintStmt(std::unique_ptr<Expr>&& message) : Stmt{ NodeKind::PRINT_STMT } { AddChildNode(std::move(message)); }
            virtual ~PrintStmt() { }

        public:
            /*
            * \fn       GetMessage
            * \brief    Gets the message to be printed on the standard output
            * \return   Message (String or Int)
            */
            const Expr* GetMessage() const { assert(mChildren.size() == 1); return GetChildNodeAs<Expr>(0); }
        };

        /*
        * \class WhileStmt
        * \brief Node of the AST representing a WHILE
        */
        class WhileStmt : public Stmt
        {
        public:
            WhileStmt(std::unique_ptr<Expr>&& condExpr, std::unique_ptr<CompoundStmt>&& body) :
                Stmt{ NodeKind::WHILE_STMT }
            {
                assert(condExpr != nullptr);
                assert(body     != nullptr);

                AddChildNode(std::move(condExpr));
                AddChildNode(std::move(body));
            }

            virtual ~WhileStmt() { }

        public:
            /*
            * \fn       GetCondExpr
            * \brief    Gets the conditional expression evaluated in the while node
            * \return   Conditional expression of the while node
            */
            const Expr* GetCondExpr() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(0); }

            /*
            * \fn       GetBody
            * \brief    Gets the actions performed by the while node when its condition evaluates to true
            * \return   Expression of the action to be performed by the while node
            */
            const Expr* GetBody() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(1); }
        };
    }
}

#endif // STMT_H__TOSTITOS
