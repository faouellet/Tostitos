#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include <string>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        enum Type
        {
            BOOL, INT, UNKNOWN
        };

        struct Symbol
        {
            Type mType;
            // TODO: Scope
			Symbol() : mType{ Type::UNKNOWN } { };
			explicit Symbol(Type t) : mType{ t } { }
        };

        class SymbolTable
        {
        public:
            SymbolTable();
            ~SymbolTable() = default;

        public:
            bool AddSymbol(const std::string& varName, const Symbol& sym);
            void AddSymbol(int value);
            bool GetSymbol(const std::string& varName, Symbol& sym);

        private:
            std::unordered_map<std::string, Symbol> mSymTable;
        };
    }
}

#endif // SYMBOLS_H__TOSTITOS
