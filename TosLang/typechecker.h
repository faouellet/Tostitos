#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "astvisitor.h"
#include "symboltable.h"

#include <map>
#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        class TypeChecker : public Utils::ASTVisitor<TypeChecker>
        {
            friend class Utils::ASTVisitor<TypeChecker>;

        public:
            TypeChecker() = default;
            ~TypeChecker() = default;

        public:
            unsigned TypeCheck(const std::unique_ptr<ASTNode>& root);

        protected:  // Declarations
            void HandleVarDecl();

        private:
            unsigned mErrorCount;
        };
    }
}

#endif // TYPE_CHECKER_H__TOSTITOS
