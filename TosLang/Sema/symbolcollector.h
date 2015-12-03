#ifndef SYMBOL_COLLECTOR_H__TOSTITOS
#define SYMBOL_COLLECTOR_H__TOSTITOS

#include "../Utils/astvisitor.h"
#include "symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        class SymbolCollector : public Utils::ASTVisitor<SymbolCollector>
        {
            friend class Utils::ASTVisitor<SymbolCollector>;

        public:
            explicit SymbolCollector(const std::shared_ptr<SymbolTable>& symTab);

        public:
            void Collect(const std::unique_ptr<ASTNode>& root);

        protected:  // Declarations
            /*
            * \fn       HandleFunctionDecl
            * \brief    Collects symbols related to a function
            */
            void HandleFunctionDecl();

            /*
            * \fn       HandleParamVarDecl
            * \brief    Collects symbols related to a function's parameters
            */
            void HandleParamVarDecl();
            
            /*
            * \fn       HandleVarDecl
            * \brief    Collects symbols related to a variable
            */
            void HandleVarDecl();

        private:
            size_t mCurrentScopeLevel;                  /*!< Current scope nesting level */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the symbol collector */
        };
    }
}

#endif // SYMBOL_COLLECTOR_H__TOSTITOS
