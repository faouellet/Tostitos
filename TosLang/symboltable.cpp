#include "symboltable.h"

using namespace TosLang::FrontEnd;

std::map<std::string, Symbol> SymbolTable::mSymTable = 
{
    { "False", Symbol(BOOL) },
    { "True",  Symbol(BOOL) }
};

bool SymbolTable::AddSymbol(const std::string& varName, const Symbol& sym)
{
    auto result = mSymTable.insert(std::make_pair(varName, sym));

    return result.second;
}

void SymbolTable::AddSymbol(int value)
{
    mSymTable.insert(std::make_pair(std::to_string(value), Symbol(INT)));
}

bool SymbolTable::GetSymbol(const std::string& varName, Symbol& sym)
{
    auto it = mSymTable.find(varName);
    if (it != mSymTable.end())
    {
        sym = it->second;
        return true;
    }

    return false;
}
