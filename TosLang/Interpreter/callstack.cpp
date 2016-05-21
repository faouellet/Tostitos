#include "callstack.h"

using namespace TosLang;

void StackFrame::AddOrUpdateValue(const FrontEnd::ASTNode* node, IntepretedValue value)
{
    mCurrentVals[node] = value;
}

bool StackFrame::TryGetNodeValue(const FrontEnd::ASTNode* node, IntepretedValue& value)
{
    auto valIt = mCurrentVals.find(node);
    const bool found = valIt != mCurrentVals.end();

    if (found)
        value = valIt->second;

    return found;
}

