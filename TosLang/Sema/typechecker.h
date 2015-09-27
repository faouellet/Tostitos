#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "../Utils/astvisitor.h"
#include "../Parse/symboltable.h"

#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        class TypeChecker : public Utils::ASTVisitor<TypeChecker>
        {
            friend class Utils::ASTVisitor<TypeChecker>;

        public:
			TypeChecker(const std::shared_ptr<SymbolTable>& symTab) : mSymbolTable(symTab), mErrorCount{ 0 } { }
            ~TypeChecker() = default;

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
