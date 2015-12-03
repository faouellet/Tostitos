#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include <string>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        enum class Type
        {
            BOOL, FUNCTION, INT, UNKNOWN
        };

        struct Symbol
        {
            Type mType;
            size_t mScopeLevel;

            Symbol() : mType{ Type::UNKNOWN }, mScopeLevel{ 0 } { };
            Symbol(Type t, size_t scopeLevel) : mType{ t }, mScopeLevel{ scopeLevel } { }
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
