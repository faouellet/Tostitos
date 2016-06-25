#ifndef SYMBOL_TABLE_H__TOSTITOS
#define SYMBOL_TABLE_H__TOSTITOS

#include "symbol.h"

#include <deque>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class CallExpr;
        class IdentifierExpr;
        class VarDecl;

        /*
        * \class SymbolTable
        * \brief Manages the symbols produced by the compiler. 
        *        Concretly, this is where the symbols produced by the symbol collection pass are put. 
        *        They are then later queried by the type checker and the instruction selector.
        */
        class SymbolTable
        {
        public:
            /*
            * \fn           AddSymbol
            * \brief        Add a symbol to the symbol table
            * \param node   Node producing the symbol (either a function or variable declaration)
            * \param sym    Symbol to be added to the symbol table
            * \return       True if the symbol was successfully added
            */
            bool AddSymbol(const ASTNode* node, Symbol&& sym);
            
            /*
            * \fn               AddVariableUse
            * \brief            Add a usage for a variable
            * \param cExpr      Identifier expression
            * \param scopePath  Path from the identifier's scope to the global scope
            * \return           True if the use could be added, else false
            */
            bool AddVariableUse(const IdentifierExpr* iExpr, const std::deque<size_t>& scopePath);
            
            /*
            * \fn           AddFunctionUse
            * \brief        Add a usage for a function
            * \param cExpr  Function call expression
            * \param fnSym  Symbol of the function being called
            * \return       True if the use could be added, else false
            */
            bool AddFunctionUse(const CallExpr* cExpr, const Symbol& fnSym);

            /*
            * \fn       Clear
            * \brief    Clears all the internal structures maintained by the symbol table
            */
            void Clear() { mTable.clear(); }

            /*
            * \fn           GetSymbol
            * \brief        Gets the symbol associated to a node
            * \param node   Node of the symbol (either a function or variable declaration)
            * \return       True and a pointer to the symbol if the symbol was found.
            */
            std::pair<bool, const Symbol*> TryGetSymbol(const ASTNode* node) const;

            /*
            * \fn               GetOverloadCandidates
            * \brief            Gets the symbols that could resolve a function call
            * \param fnName     Name of the function called
            * \return           Overload set for the function call
            */
            std::vector<const Symbol*> GetOverloadCandidates(const std::string& fnName) const;
            
            /*
            * \fn           GetFunctionDecl
            * \brief        Gets the function declaration which matches the given symbol
            * \param fnSym  Symbol of the desired function
            * \return       Pointer to the function declaration node. Nullptr if no matching declaration is found
            */
            const ASTNode* GetFunctionDecl(const Symbol& fnSym) const;

            /*
            * \fn           GetFunctionDecl
            * \brief        Gets the function declaration which is referred to by the given call expression node
            * \param fnSym  Call expression node
            * \return       Pointer to the function declaration node. Nullptr if no matching declaration is found
            */
            const ASTNode* GetFunctionDecl(const ASTNode* identExpr) const;

            /*
            * \fn           GetVarDecl
            * \brief        Gets the variable declaration which is referred to by the given identifier expression node
            * \param fnSym  Identifier expression node
            * \return       Pointer to the variable declaration node. Nullptr if no matching declaration is found
            */
            const ASTNode* GetVarDecl(const ASTNode* identExpr) const;
            
            /*
            * \fn           IsFunctionNameValid
            * \brief        Checks if a function with the given name is present in the symbol table
            * \param sym    Function name
            * \return       True if the function was found.
            */
            bool IsFunctionNameValid(const std::string& fnName) const;

            /*
            * \fn           IsFunctionSymbolValid
            * \brief        Checks if the symbol is present in the symbol table
            * \param sym    Function symbol to verify
            * \return       True if the symbol was found.
            */
            bool IsFunctionSymbolValid(const Symbol& fnSym) const;
            
            /*
            * \fn           IsVariableSymbolValid
            * \brief        Checks if the symbol is present in the symbol table
            * \param sym    Variable symbol to verify
            * \return       True if the symbol was found.
            */
            bool IsVariableSymbolValid(const Symbol& varSym) const;

            /*
            * \fn           IsGlobalVariable
            * \brief        Indicates if the variable was declared at global scope
            * \param node   Node of the variable (either an identifier expression or variable declaration)
            * \return       True if the variable was declared at global scope, else false
            */
            bool IsGlobalVariable(const ASTNode* var) const;
            
        private:
            using SymTable = std::unordered_map<const ASTNode*, Symbol>;
            using UseDefMap = std::unordered_map<const ASTNode*, const ASTNode*>;
            
        private:
            SymTable mTable;            /*!< Table containing all symbol defined in the program */
            UseDefMap mUseDefs;         /*!< Mapping between the uses and the definition of a variable */
        };
    }
}

#endif // SYMBOL_TABLE_H__TOSTITOS
