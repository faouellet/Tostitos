#include "overloadsolver.h"

#include "../AST/expressions.h"
#include "../Sema/symboltable.h"

#include <algorithm>
#include <iterator>

using namespace TosLang::FrontEnd;

size_t OverloadSolver::Run(const std::unique_ptr<ASTNode>& root, 
                           const std::shared_ptr<SymbolTable>& symTab, 
                           const std::shared_ptr<NodeTypeMap>& nodeTypes)
{
    mErrorCount = 0;
    mSymTable = symTab;
    mNodeTypes = nodeTypes;

    this->VisitPostOrder(root);
    
    return mErrorCount;
}

void OverloadSolver::HandleCallExpr()
{
    const CallExpr* cExpr = dynamic_cast<const CallExpr*>(this->mCurrentNode);
    assert(cExpr != nullptr);

    // Build the expected symbol for the function being called
    std::vector<Common::Type> argTypes;
    std::transform(cExpr->GetArgs().begin(), cExpr->GetArgs().end(), std::back_inserter(argTypes), 
                   [this](const std::unique_ptr<ASTNode>& node) 
                   { 
                       return (*mNodeTypes)[node.get()];
                   });
 
    Symbol expectedSym{ argTypes, 0, cExpr->GetCalleeName() };

    if (!mSymTable->IsFunctionSymbolValid(expectedSym))
        ++mErrorCount;    
}
