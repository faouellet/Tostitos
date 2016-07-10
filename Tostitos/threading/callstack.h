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

namespace Threading
{
    namespace impl
    {
        class StackFrame
        {
        public:
            StackFrame() : mCaller{ nullptr }, mReturnValue{}, mCurrentSymbolVals{}, mID{ mCount++ } { }
            explicit StackFrame(const TosLang::FrontEnd::ASTNode* caller) : mCaller{ caller }, mCurrentSymbolVals{} { }

        public:
            void AddOrUpdateSymbolValue(const TosLang::FrontEnd::Symbol* sym, const InterpretedValue& value);
            void SetExprValue(const TosLang::FrontEnd::ASTNode* expr, const InterpretedValue& value);
            bool TryGetSymbolValue(const TosLang::FrontEnd::Symbol* sym, InterpretedValue& value);
            bool TryGetExprValue(const TosLang::FrontEnd::ASTNode* node, InterpretedValue& value);
            const TosLang::FrontEnd::ASTNode* GetCaller() const { return mCaller; }
            const InterpretedValue& GetReturnValue() const { return mReturnValue; }
            void SetReturnValue(const InterpretedValue& val) { mReturnValue = val; }
            size_t GetID() const { return mID; }

        private:
            static size_t mCount;
            size_t mID;
            const TosLang::FrontEnd::ASTNode* mCaller;
            InterpretedValue mReturnValue;
            std::unordered_map<const TosLang::FrontEnd::Symbol*, InterpretedValue> mCurrentSymbolVals;
            std::unordered_map<const TosLang::FrontEnd::ASTNode*, InterpretedValue> mExprVals;
        };

        class CallStack
        {
        public:
            void AddOrUpdateSymbolValue(const TosLang::FrontEnd::Symbol* sym, const InterpretedValue& value, bool isGlobalSymol);
            void SetExprValue(const TosLang::FrontEnd::ASTNode* expr, const InterpretedValue& value, size_t frameID);
            bool TryGetSymbolValue(const TosLang::FrontEnd::Symbol* sym, InterpretedValue& value, bool isGlobalSymol);
            bool TryGetExprValue(const TosLang::FrontEnd::ASTNode* expr, InterpretedValue& value);

            void PushFrame(const StackFrame& frame);
            void PushFrame(StackFrame&& frame);
            void ExitCurrentFrame() { mFrames.pop_back(); }

            void Clear() { mFrames.clear(); }
            const std::deque<StackFrame>& Dump() const { return mFrames; }
            const TosLang::FrontEnd::ASTNode* GetCurrentFrameCaller() const { return mFrames.back().GetCaller(); }
            const InterpretedValue& GetReturnValue() const { return mFrames.back().GetReturnValue(); }
            void SetReturnValue(const InterpretedValue& val) { mFrames.back().SetReturnValue(val); }
            bool Empty() const { return mFrames.empty(); }
            const StackFrame& GetGlobalFrame() const { assert(!Empty()); return mFrames.front(); }

            size_t GetCurrentFrameID() const { assert(!Empty()); return mFrames.back().GetID(); }

        private:
            std::deque<StackFrame> mFrames;
        };
    }   // namespace impl
}   // namespace Threading

#endif // CALL_STACK_H__TOSTITOS
