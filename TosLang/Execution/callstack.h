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
        StackFrame() : mCaller{ nullptr }, mReturnValue{ }, mCurrentVals{} { }
        explicit StackFrame(const TosLang::FrontEnd::ASTNode* caller) : mCaller{ caller }, mCurrentVals{ } { }

    public:
        void AddOrUpdateSymbolValue(const TosLang::FrontEnd::Symbol* sym, const InterpretedValue& value);
        bool TryGetSymbolValue(const TosLang::FrontEnd::Symbol* sym, InterpretedValue& value);
        const TosLang::FrontEnd::ASTNode* GetCaller() const { return mCaller; }
        const InterpretedValue& GetReturnValue() const { return mReturnValue; }
        void SetReturnValue(const InterpretedValue& val) { mReturnValue = val; }

    private:
        const TosLang::FrontEnd::ASTNode* mCaller;
        InterpretedValue mReturnValue;
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
        const TosLang::FrontEnd::ASTNode* GetCurrentFrameCaller() const { return mFrames.back().GetCaller(); }
        const InterpretedValue& GetReturnValue() const { return mFrames.back().GetReturnValue(); }
        void SetReturnValue(const InterpretedValue& val) { mFrames.back().SetReturnValue(val); }
        bool Empty() const { return mFrames.empty(); }

    private:
        std::deque<StackFrame> mFrames;
    };
}

#endif // CALL_STACK_H__TOSTITOS
