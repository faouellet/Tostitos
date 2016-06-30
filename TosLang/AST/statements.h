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
            virtual ~Stmt() = default;
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
            virtual ~CompoundStmt() = default;

        public:
            /*
            * \fn           AddStatement
            * \brief        Add a statement to the compound statement
            * \param stmt   Statement to be added
            */
            void AddStatement(std::unique_ptr<ASTNode>&& stmt) { AddChildNode(std::move(stmt)); }

            /*
            * \fn       GetStatements
            * \brief    Gets the list of statements inside a compound statement
            * \return   The statements contained within a compound statement i.e. a scope
            */
            const ChildrenNodes& GetStatements() const { return mChildren; }
        };

        /*
        * \class IfStmt
        * \brief Node of the AST representing a IF
        */
        class IfStmt : public Stmt
        {
        public:
            IfStmt() : Stmt{ NodeKind::ERROR } { }
            IfStmt(std::unique_ptr<Expr>&& condExpr, std::unique_ptr<CompoundStmt>&& thenStmt, const Utils::SourceLocation& srcLoc) :
                Stmt{ NodeKind::IF_STMT }
            {
                assert(condExpr != nullptr);
                assert(thenStmt != nullptr);

                mSrcLoc = srcLoc;

                AddChildNode(std::move(condExpr));
                AddChildNode(std::move(thenStmt));
            }

            virtual ~IfStmt() = default;

        public:
            /*
            * \fn       GetCondExpr
            * \brief    Gets the conditional expression evaluated in the if node
            * \return   Conditional expression of the if node
            */
            const Expr* GetCondExpr() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(0); }

            /*
            * \fn       GetBody
            * \brief    Gets the actions performed by the if node when its condition evaluates to true
            * \return   Statements representing the actions to be performed by the if node when it takes the then branch
            */
            const CompoundStmt* GetBody() const { assert(mChildren.size() == 2); return GetChildNodeAs<CompoundStmt>(1); }
        };

        /*
        * \class PrintStmt
        * \brief Node of the AST representing a PRINT
        */
        class PrintStmt : public Stmt
        {
        public:
            PrintStmt(const Utils::SourceLocation& srcLoc, NodeKind kind = NodeKind::PRINT_STMT) : Stmt { NodeKind::ERROR }
            {
                assert(kind == NodeKind::PRINT_STMT || kind == NodeKind::ERROR);
                mKind = kind;
                mSrcLoc = srcLoc; 
            }

            virtual ~PrintStmt() = default;

        public:
            /*
            * \fn           AddMessage
            * \brief        Adds a message to be ouputted by the print statement
            * \param msg    Message to be printed
            */
            void AddMessage(std::unique_ptr<Expr>&& msg) { AddChildNode(std::move(msg)); }

            /*
            * \fn       SetErrorMessage
            * \brief    Sets the print statement to ERROR because of its incorrect message
            */
            void SetErrorMessage() { mKind = NodeKind::ERROR; }

            /*
            * \fn       GetMessage
            * \brief    Gets the message to be printed on the standard output
            * \return   Message (String or Int)
            */
            const Expr* GetMessage() const { assert(mChildren.size() == 1); return GetChildNodeAs<Expr>(0); }
        };

        /*
        * \class ReturnStmt
        * \brief Node of the AST representing a RETURN
        */
        class ReturnStmt : public Stmt
        {
        public:
            explicit ReturnStmt(const Utils::SourceLocation& srcLoc) : Stmt{ NodeKind::RETURN_STMT } { mSrcLoc = srcLoc; }
            virtual ~ReturnStmt() = default;

        public:
            /*
            * \fn                   AddReturnValue
            * \brief                Sets the value to be returned by the statement
            * \param returnValue    Expression representing the returned value
            * \return               true if the initialization expression could be added, false otherwise
            */
            bool AddReturnValue(std::unique_ptr<Expr>&& returnValue) 
            {
                if (returnValue == nullptr)
                {
                    return false;
                }
                else
                {
                    AddChildNode(std::move(returnValue));
                    return true;
                }
            }

            /*
            * \fn       GetReturnExpr
            * \brief    Gets the returned value expression linked to the return statement
            * \return   Pointer to the returned value expression
            */
            const Expr* GetReturnExpr() const { return mChildren.size() > 0 ? GetChildNodeAs<Expr>(0) : nullptr; }
        };

        /*
        * \class ScanStmt
        * \brief Node of the AST representing a SCAN
        */
        class ScanStmt : public Stmt
        {
        public:
            ScanStmt() : Stmt{ NodeKind::ERROR } { }
            ScanStmt(std::unique_ptr<IdentifierExpr>&& message, const Utils::SourceLocation& srcLoc)
                : Stmt{ NodeKind::SCAN_STMT } 
            {
                mSrcLoc = srcLoc;
                AddChildNode(std::move(message)); 
            }
            virtual ~ScanStmt() = default;

        public:
            /*
            * \fn       GetInput
            * \brief    Gets the input to be scanned from the standard output
            * \return   Input (String or Int)
            */
            const IdentifierExpr* GetInput() const { assert(mChildren.size() == 1); return GetChildNodeAs<IdentifierExpr>(0); }
        };

        /*
        * \class SleepStmt
        * \brief Node of the AST representing a SLEEP
        */
        class SleepStmt : public Stmt
        {
        public:
            SleepStmt() : Stmt{ NodeKind::ERROR } { }
            SleepStmt(std::unique_ptr<Expr>&& count, const Utils::SourceLocation& srcLoc)
                : Stmt{ NodeKind::SLEEP_STMT } 
            {
                mSrcLoc = srcLoc;
                AddChildNode(std::move(count));
            }
            virtual ~SleepStmt() = default;
        };

        /*
        * \class SyncStmt
        * \brief Node of the AST representing a SYNC
        */
        class SyncStmt : public Stmt
        {
        public:
            SyncStmt(const Utils::SourceLocation& srcLoc) : Stmt{ NodeKind::SYNC_STMT } 
            {
                mSrcLoc = srcLoc;
            }
            virtual ~SyncStmt() = default;
        };

        /*
        * \class WhileStmt
        * \brief Node of the AST representing a WHILE
        */
        class WhileStmt : public Stmt
        {
        public:
            WhileStmt() : Stmt{ NodeKind::ERROR } { }
            WhileStmt(std::unique_ptr<Expr>&& condExpr, std::unique_ptr<CompoundStmt>&& body, const Utils::SourceLocation& srcLoc) :
                Stmt{ NodeKind::WHILE_STMT }
            {
                assert(condExpr != nullptr);
                assert(body     != nullptr);

                mSrcLoc = srcLoc;

                AddChildNode(std::move(condExpr));
                AddChildNode(std::move(body));
            }

            virtual ~WhileStmt() = default;

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
            const CompoundStmt* GetBody() const { assert(mChildren.size() == 2); return GetChildNodeAs<CompoundStmt>(1); }
        };
    }
}

#endif // STMT_H__TOSTITOS
