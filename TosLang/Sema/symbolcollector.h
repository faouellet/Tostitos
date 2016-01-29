#ifndef SYMBOL_COLLECTOR_H__TOSTITOS
#define SYMBOL_COLLECTOR_H__TOSTITOS

#include "../AST/declarations.h"
#include "../Common/astvisitor.h"
#include "symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class SymbolCollector
        * \brief AST pass that fills the program's symbol table with informations on variables and functions
        */
        class SymbolCollector : public Common::ASTVisitor<SymbolCollector>
        {
            friend class Common::ASTVisitor<SymbolCollector>;

        public:
            explicit SymbolCollector(const std::shared_ptr<SymbolTable>& symTab);

        public:
            /*
            * \fn           Run
            * \param root   Root of the tree to print
            * \brief        Recursively walk the tree rooted at root to collect symbols for the symbol table
            * \return       Number of errors encountered during symbol collecting
            */
            size_t Run(const std::unique_ptr<ASTNode>& root);

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
            constexpr static int M_GLOBAL_SCOPE_LEVEL = 0;

        private:
            size_t mCurrentScopeLevel;                  /*!< Current scope nesting level */
            int mCurrentScopeID;                        /*!< Current scope identifier */
            size_t mErrorCount;                         /*!< Number of errors found by the symbol collector */
            FunctionDecl* mCurrentFunc;                 /*!< Current traversed function */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the symbol collector */
        };
    }
}

#endif // SYMBOL_COLLECTOR_H__TOSTITOS
