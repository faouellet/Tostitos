#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include "../Common/type.h"

#include <cassert>
#include <string>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \struct Symbol
        * \brief A symbol is a collection of informations about an AST node as it appears in the source code
        */
        class Symbol
        {
        public:
            Symbol() : mType{ Common::Type::UNKNOWN }, mScopeID{ -1 }, mIsFunction{ false } { };
            Symbol(Common::Type t, int scopeID, const std::string& name) : mType{ t }, mScopeID{ scopeID }, mIsFunction{ false }, mName{ name } { }
            Symbol(std::vector<Common::Type> ts, int scopeID, const std::string& name) : mType{ ts }, mScopeID{ scopeID }, mIsFunction{ true }, mName{ name } { }

            const Common::Type GetVariableType() const { assert(!mIsFunction);  return mType[0]; }
            const Common::Type GetFunctionReturnType() const { assert(mIsFunction);  return mType[0]; }
            const std::vector<Common::Type> GetFunctionParamTypes() const 
            {
                assert(mIsFunction);
                return std::vector<Common::Type>{mType.begin() + 1, mType.end()};
            }

            const int GetScopeID() const { return mScopeID; }
            const bool IsFunction() const { return mIsFunction; }
            const std::string& GetName() const { return mName; }

        private:
            std::vector<Common::Type> mType;    /*!< Type of the symbol
                                                For variable, it is simply the variable type
                                                For function, the first type is the return type and the others are its parameters types*/
            int mScopeID;                       /*!< Scope in which the symbol was declared */
            bool mIsFunction;                   /*!< Is the symbol representing a function */
            std::string mName;                  /*!< Name of the symbol in the source code */
        };

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
            * \fn               AddLocalSymbol
            * \brief            Add symbol local to a function to the symbol table
            * \param fnName     Name of the function in which the symbol is defined
            * \param varName    Name of the variable associated with the symbol
            * \param sym        Symbol to be added to the symbol table
            * \return           True if the symbol was successfully added
            */
            bool AddLocalSymbol(const std::string& fnName, const std::string& varName, Symbol&& sym);

            /*
            * \fn               AddGlobalSymbol
            * \brief            Add symbol global to the program to the symbol table
            * \param symName    Name of the symbol (either a function or variable)
            * \param sym        Symbol to be added to the symbol table
            * \return           True if the symbol was successfully added
            */
            bool AddGlobalSymbol(const std::string& symName, Symbol&& sym);
            
            /*
            * \fn       Clear
            * \brief    Clears all the internal structures maintained by the symbol table
            */
            void Clear();

            /*
            * \fn                       GetSymbol
            * \brief                    Gets the symbol associated to a name
            * \param fnName             Name of the function in which the symbol is defined. Will be empty in case of a global symbol.
            *                           (All functions are global symbols as are variable declared at global scope)
            * \param symName            Name of the symbol (either a function or variable)
            * \param scopesToSearch     Scopes in which the symbol might be defined
            * \param sym                Symbol to be added to the symbol table
            * \return                   True if the symbol was found.
            */
            bool GetSymbol(const std::string& fnName, const std::string& symName, const std::stack<int>& scopesToSearch, Symbol& sym);

            /*
            * \fn               IsGlobalVariable
            * \brief            Indicates if the variable was declared at global scope
            * \param varName    Name of the variable
            * \return           True if the variable was declared at global scope, else false
            */
            const bool IsGlobalVariable(const std::string& varName) const;

        private:
            using SymTable = std::unordered_map<std::string, Symbol>;
            using LocalSymTables = std::unordered_map<std::string, SymTable>;
            
        private:
            SymTable mGlobalTable;          /*!< Table containing all symbol defined in the global scope */
            LocalSymTables mLocalTables;    /*!< Tables containing symbols defined in a given local scope i.e. in a function */
        };
    }
}

#endif // SYMBOLS_H__TOSTITOS
