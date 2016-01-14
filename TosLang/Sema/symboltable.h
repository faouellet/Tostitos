#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include <string>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../Common/type.h"

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \struct Symbol
        * \brief TODO
        */
        class Symbol
        {
        public:
            Symbol() : mType{ Common::Type::UNKNOWN }, mScopeID{ -1 } { };
            Symbol(Common::Type t, int scopeID) : mType{ t }, mScopeID{ scopeID } { }
            Symbol(std::vector<Common::Type> ts, int scopeID) : mType{ ts }, mScopeID{ scopeID } { }

            const Common::Type GetVariableType() const { return mType[0]; }
            const Common::Type GetFunctionReturnType() const { return mType[0]; }
            const std::vector<Common::Type> GetFunctionParamTypes() const 
            {
                return std::vector<Common::Type>{mType.begin() + 1, mType.end()};
            }

            const int GetScopeID() const { return mScopeID; }

            // TODO: Add boolean to indicate if the symbol refer to a variable or a function
        private:
            std::vector<Common::Type> mType;    /*!< Type of the symbol
                                                For variable, it is simply the variable type
                                                For function, the first type is the return type and the others are its parameters types*/
            int mScopeID;
        };

        /*
        * \class SymbolTable
        * \brief TODO
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
            * \fn                       GetSymbol
            * \brief                    Gets the symbol associated to a name
            * \param fnName             Name of the function in which the symbol is defined. Will be empty in case of a global symbol.
            * \param symName            Name of the symbol (either a function or variable)
            * \param scopesToSearch     Scopes in which the symbol might be defined
            * \param sym                Symbol to be added to the symbol table
            * \return                   True if the symbol was found.
            */
            bool GetSymbol(const std::string& fnName, const std::string& symName, const std::stack<int>& scopesToSearch, Symbol& sym);

            bool IsGlobalVariable(const std::string& varName) const;

        private:
            using SymTable = std::unordered_map<std::string, Symbol>;
            using LocalSymTables = std::unordered_map<std::string, SymTable>;
            
            SymTable mGlobalTable;          /*!< Table containing all symbol defined in the global scope */
            LocalSymTables mLocalTables;    /*!< Tables containing symbols defined in a given local scope i.e. in a function */
        };
    }
}

#endif // SYMBOLS_H__TOSTITOS
