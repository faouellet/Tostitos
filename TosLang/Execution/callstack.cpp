#include "callstack.h"

using namespace Execution;
using namespace TosLang;

////////// Stack Frame //////////

void StackFrame::AddOrUpdateSymbolValue(const FrontEnd::Symbol* sym, const InterpretedValue& value)
{
    mCurrentVals[sym] = value;
}

bool StackFrame::TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value)
{
    auto valIt = mCurrentVals.find(sym);
    const bool found = valIt != mCurrentVals.end();

    if (found)
        value = valIt->second;

    return found;
}

////////// Call Stack //////////
void CallStack::AddOrUpdateSymbolValue(const FrontEnd::Symbol* sym, const InterpretedValue& value, bool isGlobalSymol)
{
    if (isGlobalSymol)
        mFrames.front().AddOrUpdateSymbolValue(sym, value);
    else
        mFrames.back().AddOrUpdateSymbolValue(sym, value);
}

void CallStack::EnterNewFrame(const FrontEnd::ASTNode* fnNode)
{
    mFrames.push_back(StackFrame(fnNode));
}

bool CallStack::TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value, bool isGlobalSymol)
{
    if (isGlobalSymol)
        return mFrames.front().TryGetSymbolValue(sym, value);
    else
        return mFrames.back().TryGetSymbolValue(sym, value);
}
