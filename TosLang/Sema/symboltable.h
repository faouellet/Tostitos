#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include "../Common/type.h"

#include <cassert>
#include <deque>
#include <string>
#include <string>
#include <unordered_map>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class CallExpr;
        class IdentifierExpr;
        class VarDecl;

        /*
        * \struct Symbol
        * \brief A symbol is a collection of informations about an AST node as it appears in the source code
        */
        class Symbol
        {
        public:
            Symbol() : mType{ Common::Type::ERROR }, mScopeID{ 0 }, mIsFunction{ false } { };
            Symbol(Common::Type t, size_t scopeID, const std::string& name) : mType{ t }, mScopeID{ scopeID }, mIsFunction{ false }, mName{ name } { }
            Symbol(std::vector<Common::Type> ts, size_t scopeID, const std::string& name) : mType{ ts }, mScopeID{ scopeID }, mIsFunction{ true }, mName{ name } { }

            const Common::Type GetVariableType() const { assert(!mIsFunction);  return mType[0]; }
            const Common::Type GetFunctionReturnType() const { assert(mIsFunction);  return mType[0]; }
            std::vector<Common::Type> GetFunctionParamTypes() const 
            {
                assert(mIsFunction);

                if (mType.size() == 1)
                    // No parameters, only a return type
                    return{};
                else
                    return { ++mType.begin(), mType.end() };
            }
            
            Common::Type GetFunctionParamTypeAt(const size_t idx) const
            {
                assert(mIsFunction);
                assert(idx + 1 < mType.size());

                return mType[idx + 1];
            }

            size_t GetScopeID() const { assert(mType.front() != Common::Type::ERROR); return mScopeID; }
            bool IsFunction() const { assert(mType.front() != Common::Type::ERROR); return mIsFunction; }
            bool IsGlobal() const { return GetScopeID() == 0; }
            const std::string& GetName() const { assert(mType.front() != Common::Type::ERROR); return mName; }

        public:
            friend bool operator==(const Symbol& lhsSym, const Symbol& rhsSym)
            {
                return (lhsSym.mType == rhsSym.mType) 
                    && (lhsSym.mScopeID == rhsSym.mScopeID) 
                    && (lhsSym.mIsFunction == rhsSym.mIsFunction) 
                    && (lhsSym.mName == rhsSym.mName);
            }

        private:
            std::vector<Common::Type> mType;    /*!< Type of the symbol
                                                     For variable, it is simply the variable type
                                                     For function, the first type is the return type and the others are its parameters types */
            size_t mScopeID;                    /*!< Scope in which the symbol was declared */
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
            * \fn           FindFunctionDecl
            * \brief        Gets the function declaration which matches the given symbol
            * \param fnSym  Symbol of the desired function
            * \return       Pointer to the function AST node. Nullptr if no matching declaration is found
            */
            const ASTNode* FindFunctionDecl(const Symbol& fnSym) const;

            /*
            * TODO
            */
            const ASTNode* GetFunctionDecl(const ASTNode* identExpr) const;

            /*
            * TODO
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

#endif // SYMBOLS_H__TOSTITOS
