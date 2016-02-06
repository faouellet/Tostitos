#ifndef SCOPE_CHECKER_H__TOSTITOS
#define SCOPE_CHECKER_H__TOSTITOS

#include "../AST/declarations.h"
#include "../Common/astvisitor.h"
#include "symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class ScopeChecker
        * \brief AST pass that verifies when a variable is used or a function is called it is accessible in the current scope.
        *        A variable is inaccessible when it isn't declared in the current scope or an outer scope
        *        A function is inaccessible when it isn't declared at all (since all functions are global symbols)
        */
        class ScopeChecker : public Common::ASTVisitor<ScopeChecker>
        {
            friend class Common::ASTVisitor<ScopeChecker>;

        public:
            explicit ScopeChecker(const std::shared_ptr<SymbolTable>& symTab);

        public:
            /*
            * \fn           Run
            * \brief        Recursively walk the tree rooted at root to check for inacessible function or variable
            * \param root   Root of the tree to print
            * \return       Number of errors encountered during type checking
            */
            size_t Run(const std::unique_ptr<ASTNode>& root);

        protected:
            void HandleCallExpr();
            void HandleIdentifierExpr();

        private:
            size_t mErrorCount;                                         /*!< Number of errors found by the type checker */
            int mCurrentScopeID;                                        /*!< Current scope identifier */
            FunctionDecl* mCurrentFunc;                                 /*!< Current traversed function */
            std::stack<int> mCurrentScopesTraversed;                    /*!< Path from the current scope to the global scopes */
            std::shared_ptr<SymbolTable> mSymbolTable;                  /*!< Symbol table to be used by the type checker */
        };
    }
}

#endif // SCOPE_CHECKER_H__TOSTITOS
