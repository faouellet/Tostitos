#include "symboltable.h"

#include <algorithm>
#include <sstream>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

bool TosLang::FrontEnd::SymbolTable::AddLocalSymbol(const std::string& fnName, const std::string& varName, Symbol&& sym)
{
    // We suffix the symbol name with the scope level to allow for multiple definitions of the same variable in 
    // different scopes inside the function
    std::stringstream sStream;
    sStream << varName << sym.GetScopeID();
    std::string realSymName = sStream.str();

    auto fnIt = mLocalTables.find(fnName);
    if (fnIt == mLocalTables.end()) // First time we encounter this function name
    {
        mLocalTables[fnName][realSymName] = sym;
        return true;
    }
    else
    {
        auto symIt = fnIt->second.find(realSymName);
        
        if (symIt == fnIt->second.end()) // First time we encounter this variable name
        {
            mLocalTables[fnName][realSymName] = sym;
            return true;
        }
        else
        {
            // Exact same symbol already present in the local table, can't insert it again
            return false;
        }
    }
}

bool SymbolTable::AddGlobalSymbol(const std::string& symName, Symbol&& sym)
{
    // We suffix the symbol name with the scope level (0) to be coherent with the local symbols
    std::stringstream sStream;
    sStream << symName << 0;
    std::string realSymName = sStream.str();

    auto it = mGlobalTable.find(realSymName);
    if (it == mGlobalTable.end()) // First time we encounter this symbol name
    {
        mGlobalTable[realSymName] = sym;
        return true;
    }
    else
    {
        // Exact same symbol already present in the global table, can't insert it again
        return false;
    }
}

void TosLang::FrontEnd::SymbolTable::Clear()
{
    mLocalTables.clear();
    mGlobalTable.clear();
}

bool SymbolTable::GetSymbol(const std::string& fnName, const std::string& symName, const std::stack<int>& scopesToSearch, Symbol& sym)
{
    if (fnName.empty()) // We were asked for a global symbol
    {
        auto symIt = std::find_if(mGlobalTable.begin(), mGlobalTable.end(), [symName](const SymTable::value_type& sym) { return sym.second.GetName() == symName; });
        
        // Symbol name is not in the global table
        if (symIt == mGlobalTable.end())
            return false;

        sym = symIt->second;
        return true;
    }
    else // We were asked for a local symbol
    {
        auto fnIt = mLocalTables.find(fnName);

        // Function name is not in the global table
        if (fnIt == mLocalTables.end())
            return false;

        // Starting at the most nested scope, we search outward for the symbol
        // associated with symName
        std::stack<int> scopes{ scopesToSearch };
        while (!scopes.empty())
        {
            int scopeID = scopes.top();
            scopes.pop();

            std::stringstream sStream;
            sStream << symName << scopeID;

            auto it = fnIt->second.find(sStream.str());
            if (it != fnIt->second.end())
            {
                // We got it
                sym = it->second;
                return true;
            }
        }

        // No corresponding symbol found
        return false;
    }
}

const bool TosLang::FrontEnd::SymbolTable::IsGlobalVariable(const std::string & varName) const
{
    return mGlobalTable.find(varName) == mGlobalTable.end();
}

