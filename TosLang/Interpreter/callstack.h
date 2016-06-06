#ifndef CALL_STACK_H__TOSTITOS
#define CALL_STACK_H__TOSTITOS

#include "interpretedvalue.h"

#include <deque>
#include <unordered_map>

// TODO: Comments

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class FunctionDecl;
        class Symbol;
    }

    class StackFrame
    {
    public:
        StackFrame() : mCaller{ nullptr }, mCurrentVals{ } { }
        explicit StackFrame(const FrontEnd::ASTNode* caller) : mCaller{ caller }, mCurrentVals{ } { }

    public:
        void AddOrUpdateSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue value);
        bool TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value);

    private:
        const FrontEnd::ASTNode* mCaller;
        std::unordered_map<const FrontEnd::Symbol*, InterpretedValue> mCurrentVals;
    };
    
    class CallStack
    {
    public:
        void AddOrUpdateSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue value, bool isGlobalSymol);
        bool TryGetSymbolValue(const FrontEnd::Symbol* sym, InterpretedValue& value, bool isGlobalSymol);

        void EnterNewFrame(const FrontEnd::ASTNode* fnNode);
        void ExitCurrentFrame() { mFrames.pop_back(); }

        void Clear() { mFrames.clear(); }
        const std::deque<StackFrame>& Dump() const { return mFrames; }

    private:
        std::deque<StackFrame> mFrames;
    };
}

#endif // CALL_STACK_H__TOSTITOS
