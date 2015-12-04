#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        class TypeChecker : public Common::ASTVisitor<TypeChecker>
        {
            friend class Common::ASTVisitor<TypeChecker>;

        public:
			TypeChecker(const std::shared_ptr<SymbolTable>& symTab) : mSymbolTable(symTab), mErrorCount{ 0 } { }

        public:
            unsigned TypeCheck(const std::unique_ptr<ASTNode>& root);
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
