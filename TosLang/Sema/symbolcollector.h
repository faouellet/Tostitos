#ifndef SYMBOL_COLLECTOR_H__TOSTITOS
#define SYMBOL_COLLECTOR_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "symboltable.h"

#include <deque>

namespace TosLang
{
    namespace FrontEnd
    {
        class FunctionDecl;

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

        protected:  // Expression
            /*
            * \fn       HandleCallExpr
            * \brief    Verify if the function called was declared
            */
            void HandleCallExpr();

            /*
            * \fn       HandleIdentifierExpr
            * \brief    Associates a variable use with a variable definition
            */
            void HandleIdentifierExpr();
                        
        private:
            constexpr static int M_GLOBAL_SCOPE_LEVEL = 0;

        private:
            size_t mCurrentScopeID;                     /*!< Current scope identifier */
            size_t mErrorCount;                         /*!< Number of errors found by the symbol collector */
            const FunctionDecl* mCurrentFunc;           /*!< Current traversed function */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the symbol collector */
            std::deque<size_t> mCurrentScopesPath;      /*<! Path from the current scope to the global scope */
        };
    }
}

#endif // SYMBOL_COLLECTOR_H__TOSTITOS
