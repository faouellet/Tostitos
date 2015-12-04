#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include <string>
#include <unordered_map>

#include "../Common/type.h"

namespace TosLang
{
    namespace FrontEnd
    {
        struct Symbol
        {
            Common::Type mType;
            size_t mScopeLevel;

            Symbol() : mType{ Common::Type::UNKNOWN }, mScopeLevel{ 0 } { };
            Symbol(Common::Type t, size_t scopeLevel) : mType{ t }, mScopeLevel{ scopeLevel } { }
        };

        class SymbolTable
        {
        public:
            SymbolTable();

        public:
            void AddSymbol(const std::string& varName, const Symbol& sym);
            void ExitScope(size_t scopeLevel);
            bool GetSymbol(const std::string& varName, Symbol& sym);

        private:
            std::unordered_multimap<std::string, Symbol> mSymTable;
        };
    }
}

#endif // SYMBOLS_H__TOSTITOS
