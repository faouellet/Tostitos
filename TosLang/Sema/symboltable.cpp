#include "symboltable.h"

#include "../AST/declarations.h"

#include <algorithm>
#include <sstream>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

bool SymbolTable::AddSymbol(const ASTNode* node, Symbol&& sym)
{
    if (node == nullptr)
        return false;

    const ASTNode::NodeKind kind = node->GetKind();

    assert(kind == ASTNode::NodeKind::FUNCTION_DECL
           || kind == ASTNode::NodeKind::VAR_DECL);

    auto symIt = std::find_if(mTable.begin(), mTable.end(), 
                              [&sym](const std::pair<const ASTNode*, Symbol>& nodeSym)
                              {
                                  const Symbol& symbol = nodeSym.second;

                                  if (symbol.IsFunction() && sym.IsFunction())
                                  {
                                      // Check for every aspects of a function
                                      return symbol.GetFunctionParamTypes() == sym.GetFunctionParamTypes()
                                          && symbol.GetFunctionReturnType() == sym.GetFunctionReturnType()
                                          && symbol.GetName() == sym.GetName();
                                  }
                                  else if (!symbol.IsFunction() && !sym.IsFunction())
                                  {
                                      // Check only for the scope and the name to disallow declaring 
                                      // the same variable with another type
                                      return symbol.GetScopeID() == sym.GetScopeID()
                                          && symbol.GetName() == sym.GetName();
                                  }
                                  else
                                  {
                                      return false;
                                  }
                              });

    // Symbol was already defined
    if (symIt != mTable.end())
        return false;

    mTable[node] = sym;

    return true;
}

void SymbolTable::AddFunctionUse(const CallExpr* cExpr, const Symbol& fnSym)
{
    auto symIt = std::find_if(mTable.begin(), mTable.end(), 
                              [&fnSym](const std::pair<const ASTNode*, Symbol>& nodeSym) 
                              {
                                  return nodeSym.second == fnSym;
                              });

    // Since this method should only be use after overload resolution, 
    // there should be a function corresponding to the given symbol
    assert(symIt != mTable.end());

    mUseDefs[symIt->first] = cExpr;
}

bool SymbolTable::AddVariableUse(const IdentifierExpr* iExpr, const std::deque<size_t> scopePath)
{
    for (const auto& scopeID : scopePath)
    {
        auto varIt = std::find_if(mTable.begin(), mTable.end(), 
                     [&scopeID, &iExpr](const std::pair<const ASTNode*, Symbol>& nodeSym) 
                     {
                         return nodeSym.second.GetScopeID() == scopeID
                             && nodeSym.second.GetName() == iExpr->GetName();
                     });

        if (varIt != mTable.end())
        {
            mUseDefs[iExpr] = varIt->first;
            return true;
        }
    }

    return false;
}

bool SymbolTable::GetSymbol(const ASTNode* node, const Symbol* sym) const
{
    if (node == nullptr)
        return false;

    const ASTNode* nodeForLookup = node;
    ASTNode::NodeKind nodeKind = nodeForLookup->GetKind();

    // An identifier or a call is a use. Since the symbol table only contains information
    // about definitions, we need to find the corresponding definition to this use
    if (( nodeKind== ASTNode::NodeKind::IDENTIFIER_EXPR)
        || (nodeKind == ASTNode::NodeKind::CALL_EXPR))
    {
        auto udIt = mUseDefs.find(node);
        if (udIt != mUseDefs.end())
            nodeForLookup = udIt->second;
        else
            return false;
    }
    
    nodeKind = nodeForLookup->GetKind();

    if ((nodeKind == ASTNode::NodeKind::FUNCTION_DECL)
        || (nodeKind == ASTNode::NodeKind::VAR_DECL))
    {
        auto symIt = mTable.find(nodeForLookup);
        if (symIt != mTable.end())
        {
            sym = &symIt->second;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    // Unhandled node type
    return false;
}

bool SymbolTable::IsFunctionSymbolValid(const Symbol & fnSym) const
{
    auto fnIt = std::find_if(mTable.begin(), mTable.end(), 
                             [&fnSym](const std::pair<const ASTNode*, Symbol> nodeSym)
                             {
                                 return nodeSym.first->GetKind() == ASTNode::NodeKind::FUNCTION_DECL
                                     && nodeSym.second == fnSym;
                             });

    return fnIt != mTable.end();
}

bool SymbolTable::IsVariableSymbolValid(const Symbol & varSym) const
{
    auto fnIt = std::find_if(mTable.begin(), mTable.end(),
                             [&varSym](const std::pair<const ASTNode*, Symbol> nodeSym)
                             {
                                 return nodeSym.first->GetKind() == ASTNode::NodeKind::VAR_DECL
                                     && nodeSym.second == varSym;
                             });

    return fnIt != mTable.end();
}

bool SymbolTable::IsGlobalVariable(const ASTNode* node) const
{
    assert(node != nullptr);

    ASTNode::NodeKind nodeKind = node->GetKind();
    assert((nodeKind == ASTNode::NodeKind::IDENTIFIER_EXPR)
        || (nodeKind == ASTNode::NodeKind::VAR_DECL));

    const ASTNode* nodeForLookup = node;

    if (nodeKind == ASTNode::NodeKind::IDENTIFIER_EXPR)
    {
        auto udIt = mUseDefs.find(node);
        assert(udIt != mUseDefs.end());
        nodeForLookup = udIt->second;
    }

    auto symIt = mTable.find(nodeForLookup);
    assert(symIt != mTable.end());

    return symIt->second.GetScopeID() == 0;
}

const ASTNode* SymbolTable::FindVarDecl(const ASTNode* identExpr, const size_t scopeID) const
{
    auto identIt = std::find_if(mTable.begin(), mTable.end(), [&](const std::pair<const ASTNode*, Symbol> nodeSym) 
    { 
        return nodeSym.first->GetKind() == ASTNode::NodeKind::VAR_DECL
               && nodeSym.first->GetName() == identExpr->GetName()
               && nodeSym.second.GetScopeID() == scopeID;
    });

    if (identIt != mTable.end())
        return identIt->first;
    else
        return nullptr;
}
