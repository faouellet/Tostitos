#include "callstack.h"

using namespace TosLang;

void StackFrame::AddOrUpdateValue(const FrontEnd::Symbol* sym, InterpretedValue value)
{
    mCurrentVals[sym] = value;
}

bool StackFrame::TryGetNodeValue(const FrontEnd::Symbol* sym, InterpretedValue& value)
{
    auto valIt = mCurrentVals.find(sym);
    const bool found = valIt != mCurrentVals.end();

    if (found)
        value = valIt->second;

    return found;
}

