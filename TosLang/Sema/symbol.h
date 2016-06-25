#ifndef SYMBOL_H__TOSTITOS
#define SYMBOL_H__TOSTITOS

#include "../Common/type.h"

#include <cassert>
#include <string>
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
            /*
            * \fn       Symbol
            * \brief    Default constructor
            */
            Symbol() : mType{ Common::Type::ERROR }, mScopeID{ 0 }, mIsFunction{ false }, mSize{ 0 } { };

            /*
            * \fn               Symbol
            * \brief            Variable symbol constructor
            * \param t          Type of the variable
            * \param scopeID    ID of the scope the variable was declared in
            * \param name       Name of the variable
            * \param size       Size of the variable
            */
            Symbol(Common::Type t, size_t scopeID, const std::string& name, int size) 
                : mType{ t }, mScopeID{ scopeID }, mIsFunction{ false }, mName{ name }, mSize{ size } { }

            /*
            * \fn               Symbol
            * \brief            Function symbol constructor
            * \param t          Types of the function. The first one is the return type and all the others are the parameters' types
            * \param name       Name of the function
            */
            Symbol(std::vector<Common::Type> ts, const std::string& name) 
                : mType{ ts }, mScopeID{ 0 }, mIsFunction{ true }, mName{ name }, mSize{ 0 } { }

            /*
            * \fn       GetVariableType
            * \brief    Gets the type of the variable the symbol refers to
            * \return   The variable type
            */
            Common::Type GetVariableType() const { assert(!mIsFunction);  return mType[0]; }

            /*
            * \fn       GetFunctionReturnType
            * \brief    Gets the return type of the function the symbol refers to
            * \return   The function return type
            */
            Common::Type GetFunctionReturnType() const { assert(mIsFunction);  return mType[0]; }

            /*
            * \fn       GetFunctionParamTypes
            * \brief    Gets the types of the parameters of the function the symbol refers to
            * \return   The function's parameters' types
            */
            std::vector<Common::Type> GetFunctionParamTypes() const
            {
                assert(mIsFunction);

                if (mType.size() == 1)
                    // No parameters, only a return type
                    return{};
                else
                    return{ ++mType.begin(), mType.end() };
            }

            /*
            * \fn           GetFunctionParamTypeAt
            * \brief        Gets the type of a parameter of the function the symbol refers to
            * \param idx    Index of the function parameter
            * \return       The function's parameters' types
            */
            Common::Type GetFunctionParamTypeAt(size_t idx) const
            {
                // Must refer to a function
                assert(mIsFunction);

                // The index received is that of the parameters. However, internally, we store 
                // the return and parameters types into the same vector. We need to adjust the 
                // index accordingly and make sure the index is still valid afterwards.
                const size_t adjustedIdx = ++idx;
                assert(adjustedIdx < mType.size());

                return mType[adjustedIdx];
            }

            /*
            * \fn       GetScopeID
            * \brief    Gets the ID of the scope the symbol was declared in
            * \return   The scope ID
            */
            size_t GetScopeID() const { assert(mType.front() != Common::Type::ERROR); return mScopeID; }

            /*
            * \fn       IsFunction
            * \brief    Indicates if the symbol refers to a function
            * \return   True if the symbol refers to a function, false otherwise
            */
            bool IsFunction() const { assert(mType.front() != Common::Type::ERROR); return mIsFunction; }

            /*
            * \fn       IsGlobal
            * \brief    Indicates if the symbol was declared in the global scope
            * \return   True if the symbol was declared in the global scope, false otherwise
            */
            bool IsGlobal() const { return GetScopeID() == 0; }

            /*
            * \fn       GetName
            * \brief    Gets the name of the symbol
            * \return   Name of the symbol
            */
            const std::string& GetName() const { assert(mType.front() != Common::Type::ERROR); return mName; }

            /*
            * \fn       GetSize
            * \brief    Gets the size of the variable referred to by the symbol.
            * \return   Size of the variable. Will return 0 if the symbol refers to a scalar variable or a function
            */
            int GetSize() const { assert((mType.front() != Common::Type::ERROR) && !mIsFunction); return mSize; }

        public:
            friend bool operator==(const Symbol& lhsSym, const Symbol& rhsSym)
            {
                return (lhsSym.mType == rhsSym.mType)
                    && (lhsSym.mScopeID == rhsSym.mScopeID)
                    && (lhsSym.mIsFunction == rhsSym.mIsFunction)
                    && (lhsSym.mName == rhsSym.mName)
                    && (lhsSym.mSize == rhsSym.mSize);
            }

        private:
            std::vector<Common::Type> mType;    /*!< Type of the symbol
                                                     For variable, it is simply the variable type
                                                     For function, the first type is the return type and the others are its parameters types */
            size_t mScopeID;                    /*!< Scope in which the symbol was declared */
            bool mIsFunction;                   /*!< Is the symbol representing a function */
            std::string mName;                  /*!< Name of the symbol in the source code */
            int mSize;                          /*!< Size of the variable referred to by the symbol.
                                                     A size of 0 means that the variable is a scalar. */
        };
    }
}

#endif // SYMBOL_H__TOSTITOS
