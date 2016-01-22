#ifndef TYPE_INFER_H__TOSTITOS
#define TYPE_INFER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        class TypeInfer : public Common::ASTVisitor<TypeInfer>
        {
            friend class Common::ASTVisitor<TypeInfer>;

        public:
            explicit TypeInfer(const std::shared_ptr<SymbolTable>& symTab);
          
        public:
            void Infer(const std::unique_ptr<ASTNode>& root);

        protected:  // Declarations
            /*
            * \fn       HandleFunctionDecl
            * \brief    Infers the return type of a function
            */
            void HandleFunctionDecl();
            
            /*
            * \fn       HandleVarDecl
            * \brief    Infers the return type of a variable
            */
            void HandleVarDecl();

        private:
            size_t mCurrentScopeLevel;                  /*!< Current scope nesting level */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the symbol collector */
        };
    }
}

#endif // TYPE_INFER_H__TOSTITOS