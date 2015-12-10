#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "../AST/declarations.h"
#include "../Common/astvisitor.h"
#include "symboltable.h"

#include <stack>

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class TypeChecker
        * \brief AST pass that verifies if no type error occurs when a variable or a function is used
        */
        class TypeChecker : public Common::ASTVisitor<TypeChecker>
        {
            friend class Common::ASTVisitor<TypeChecker>;

        public:
            explicit TypeChecker(const std::shared_ptr<SymbolTable>& symTab);

        public:
            /*
            * \fn           Run
            * \brief        Recursively walk the tree rooted at root to check for type errors
            * \param root   Root of the tree to print
            * \return       Number of errors encountered during type checking
            */
            unsigned Run(const std::unique_ptr<ASTNode>& root);

            /*
            * \fn       GetErrorCount
            * \brief    Gets the current number of type errors encountered by the type checker
            * \return   Current number of type errors encountered by the type checker
            */
            unsigned GetErrorCount() const { return mErrorCount; }

        protected:  // Declarations
            void HandleVarDecl();
            void HandleFunctionDecl();

        protected:  // Expressions
            void HandleBinaryExpr();
            void HandleCallExpr();

        protected:
            void HandleIfStmt();
            void HandlePrintStmt();
            void HandleScanStmt();
            void HandleWhileStmt();

        private:
            unsigned mErrorCount;                       /*!< Number of errors found by the type checker */
            int mCurrentScopeID;                        /*!< Current scope identifier */
            FunctionDecl* mCurrentFunc;                 /*!< Current traversed function */
            std::stack<int> mCurrentScopesTraversed;    /*!< Path from the current scope to the global scopes */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be used by the type checker */
        };
    }
}

#endif // TYPE_CHECKER_H__TOSTITOS
