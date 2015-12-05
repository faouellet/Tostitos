#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "symboltable.h"

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
			TypeChecker(const std::shared_ptr<SymbolTable>& symTab) : mSymbolTable(symTab), mErrorCount{ 0 } { }

        public:
            /*
            * \fn           Run
            * \param root   Root of the tree to print
            * \brief        Recursively walk the tree rooted at root to check for type errors
            */
            unsigned Run(const std::unique_ptr<ASTNode>& root);
            unsigned GetErrorCount() const { return mErrorCount; }

        protected:  // Declarations
            void HandleVarDecl();

        private:
            unsigned mErrorCount;                       /*!< Number of errors found by the type checker */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be used by the type checker */
        };
    }
}

#endif // TYPE_CHECKER_H__TOSTITOS
