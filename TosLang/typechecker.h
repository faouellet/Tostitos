#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "astvisitor.h"
#include "symbols.h"

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
            void TypeCheck(const std::unique_ptr<ASTNode>& root);

        protected:  // Declarations
            void HandleVarDecl();

        private:
            std::map<std::string, Type> mTypeTable; /*!< Lexer used by the parser to acquire tokens */
        };
    }
}

#endif // TYPE_CHECKER_H__TOSTITOS
