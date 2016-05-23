#ifndef CALL_STACK_H__TOSTITOS
#define CALL_STACK_H__TOSTITOS

#include "interpretedvalue.h"

#include <map>
#include <stack>

// TODO: Comments

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
    }

    class StackFrame
    {
    public:
        StackFrame() : mCaller{ nullptr }, mCurrentVals{ } { }
        explicit StackFrame(const FrontEnd::ASTNode* caller) { }

    public:
        void AddOrUpdateValue(const FrontEnd::ASTNode* node, InterpretedValue value);
        bool TryGetNodeValue(const FrontEnd::ASTNode* node, InterpretedValue& value);

    private:
        const FrontEnd::ASTNode* mCaller;
        std::map<const FrontEnd::ASTNode*, InterpretedValue> mCurrentVals;
    };
    
    using CallStack = std::stack<StackFrame>;
}

#endif // CALL_STACK_H__TOSTITOS
