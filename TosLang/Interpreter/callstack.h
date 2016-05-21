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
        StackFrame(const FrontEnd::ASTNode* caller) {}

    public:
        void AddOrUpdateValue(const FrontEnd::ASTNode* node, IntepretedValue value);
        bool TryGetNodeValue(const FrontEnd::ASTNode* node, IntepretedValue& value);

    private:
        const FrontEnd::ASTNode* mCaller;
        std::map<const FrontEnd::ASTNode*, IntepretedValue> mCurrentVals;
    };
    
    using CallStack = std::stack<StackFrame>;
}

#endif // CALL_STACK_H__TOSTITOS
