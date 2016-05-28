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
        class Symbol;
    }

    class StackFrame
    {
    public:
        StackFrame() : mCaller{ nullptr }, mCurrentVals{ } { }
        explicit StackFrame(const FrontEnd::ASTNode* caller) : mCaller{ caller }, mCurrentVals{ } { }

    public:
        void AddOrUpdateValue(const FrontEnd::Symbol* sym, InterpretedValue value);
        bool TryGetNodeValue(const FrontEnd::Symbol* sym, InterpretedValue& value);

    private:
        const FrontEnd::ASTNode* mCaller;
        std::unordered_map<const FrontEnd::Symbol*, InterpretedValue> mCurrentVals;
    };
    
    using CallStack = std::deque<StackFrame>;
}

#endif // CALL_STACK_H__TOSTITOS
