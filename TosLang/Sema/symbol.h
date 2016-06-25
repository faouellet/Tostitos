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
                    return{ ++mType.begin(), mType.end() };
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
    }
}

#endif // SYMBOL_H__TOSTITOS
