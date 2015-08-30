#ifndef SYMBOLS_H__TOSTITOS
#define SYMBOLS_H__TOSTITOS

#include <map>
#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        enum Type
        {
            BOOL, INT
        };

        struct Symbol
        {
            Type mType;
            // TODO: Scope
            Symbol() = default;
            Symbol(Type t) : mType(t) { }
        };

        class SymbolTable
        {
        public:
            SymbolTable(const SymbolTable&) = delete;
            SymbolTable& operator=(const SymbolTable&) = delete;
            ~SymbolTable() = default;

        public:
            static bool AddSymbol(const std::string& varName, const Symbol& sym);
            static void AddSymbol(int value);
            static bool GetSymbol(const std::string& varName, Symbol& sym);

        private:
            /*
            * \fn       SymbolTable
            * \brief    Default constructor
            */
            SymbolTable() = default;

        private:
            static std::map<std::string, Symbol> mSymTable;
            static Symbol mInstance;
        };
    }
}

#endif // SYMBOLS_H__TOSTITOS
