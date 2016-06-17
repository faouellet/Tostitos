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
}

namespace Execution
{
    class StackFrame
    {
    public:
        StackFrame() : mCaller{ nullptr }, mCurrentVals{ } { }
        explicit StackFrame(const TosLang::FrontEnd::ASTNode* caller) : mCaller{ caller }, mCurrentVals{ } { }

    public:
        void AddOrUpdateSymbolValue(const TosLang::FrontEnd::Symbol* sym, const InterpretedValue& value);
        bool TryGetSymbolValue(const TosLang::FrontEnd::Symbol* sym, InterpretedValue& value);

    private:
        const TosLang::FrontEnd::ASTNode* mCaller;
        std::unordered_map<const TosLang::FrontEnd::Symbol*, InterpretedValue> mCurrentVals;
    };
    
    class CallStack
    {
    public:
        void AddOrUpdateSymbolValue(const TosLang::FrontEnd::Symbol* sym, const InterpretedValue& value, bool isGlobalSymol);
        bool TryGetSymbolValue(const TosLang::FrontEnd::Symbol* sym, InterpretedValue& value, bool isGlobalSymol);

        void EnterNewFrame(const TosLang::FrontEnd::ASTNode* fnNode);
        void ExitCurrentFrame() { mFrames.pop_back(); }

        void Clear() { mFrames.clear(); }
        const std::deque<StackFrame>& Dump() const { return mFrames; }

    private:
        std::deque<StackFrame> mFrames;
    };
}

#endif // CALL_STACK_H__TOSTITOS
