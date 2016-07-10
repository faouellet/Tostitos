#include "callstack.h"

#include <algorithm>

using namespace Threading::impl;
using namespace TosLang;

////////// Stack Frame //////////

size_t StackFrame::mCount = 0;

void StackFrame::AddOrUpdateSymbolValue(const FrontEnd::Symbol* sym, const InterpretedValue& value)
{
    mCurrentSymbolVals[sym] = value;
}

void StackFrame::SetExprValue(const TosLang::FrontEnd::ASTNode* expr, const InterpretedValue& value)
{
    mExprVals[expr] = value;
}

bool StackFrame::TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value)
{
    auto valIt = mCurrentSymbolVals.find(sym);
    const bool found = valIt != mCurrentSymbolVals.end();

    if (found)
        value = valIt->second;

    return found;
}

bool StackFrame::TryGetExprValue(const FrontEnd::ASTNode* expr, InterpretedValue& value)
{
    auto valIt = mExprVals.find(expr);
    const bool found = valIt != mExprVals.end();

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

void CallStack::SetExprValue(const TosLang::FrontEnd::ASTNode* expr, const InterpretedValue& value, size_t frameID)
{
    auto foundIt = std::find_if(mFrames.begin(), mFrames.end(), [&frameID](const StackFrame& frame) { return frameID == frame.GetID(); });

    if (foundIt != mFrames.end())
        foundIt->SetExprValue(expr, value);
}

void CallStack::PushFrame(const StackFrame& frame)
{
    mFrames.push_back(frame);
}

void CallStack::PushFrame(StackFrame&& frame)
{
    mFrames.emplace_back(frame);
}

bool CallStack::TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value, bool isGlobalSymol)
{
    if (isGlobalSymol)
        return mFrames.front().TryGetSymbolValue(sym, value);
    else
        return mFrames.back().TryGetSymbolValue(sym, value);
}

bool CallStack::TryGetExprValue(const FrontEnd::ASTNode* expr, InterpretedValue& value)
{
    return mFrames.back().TryGetExprValue(expr, value);
}
