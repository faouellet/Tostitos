#include "symboltable.h"

#include <algorithm>

using namespace TosLang::FrontEnd;

SymbolTable::SymbolTable()
{
    mSymTable.insert({ "False", Symbol(Type::BOOL, 0) });
    mSymTable.insert({ "True", Symbol(Type::BOOL, 0) });
}

void SymbolTable::AddSymbol(const std::string& varName, const Symbol& sym)
{
    mSymTable.insert({ varName, sym });
}

void SymbolTable::ExitScope(size_t scopeLevel)
{
    // TODO: This will not scale well. Should we make our own hash table since the bucket
    //       interface of std::unordered_multimap is lacking for our purposes?

    for (auto it = mSymTable.begin(), end = mSymTable.end(); it != end; )
        if (it->second.mScopeLevel == scopeLevel)
            it = mSymTable.erase(it);
        else
            ++it;
}

bool SymbolTable::GetSymbol(const std::string& varName, Symbol& sym)
{
    auto range = mSymTable.equal_range(varName);

    // No matching symbol found
    if (range.first == range.second)
        return false;

    // Sort in descending order since the scope level is incremented each time we enter a new scope.
    //std::sort(range.first, range.second, [](const Symbol& sym1, const Symbol& sym2) { return sym1.mScopeLevel > sym2.mScopeLevel; });

    // Get the correspoing symbol in the scope closest to the current one
    sym = range.first->second;

    return true;
}
