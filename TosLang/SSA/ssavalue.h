#ifndef SSA_VALUE__TOSTITOS
#define SSA_VALUE__TOSTITOS

#include <iostream>

namespace TosLang
{
    namespace BackEnd
    {
        class SSAInstruction;

        /*
        * TODO
        */
        class SSAValue
        {
        public:
            enum class ValueKind
            {
                UNKNOWN,    // I am error
                ARGUMENT,   // Function argument
                LITERAL,    // Literal value such as an integer or a char
                RESULT,     // Result of an instruction
            };

        public:
            SSAValue() : mKind{ ValueKind::UNKNOWN }, mID{ 0 }, mLitVal{ 0 } { }
            SSAValue(size_t id) : mKind{ ValueKind::ARGUMENT }, mID{ id }, mLitVal{ 0 } { }
            SSAValue(size_t id, int constVal) : mKind{ ValueKind::LITERAL }, mID{ id }, mLitVal{ constVal } { }
            virtual ~SSAValue() = default;

        public:
            friend std::ostream& operator<<(std::ostream& stream, const SSAValue& ssaVal);
            friend bool operator==(const SSAValue& lhsVal, const SSAValue& rhsVal);
            friend bool operator!=(const SSAValue& lhsVal, const SSAValue& rhsVal);

        protected:
            ValueKind mKind;
            size_t mID;
            int mLitVal;
            SSAInstruction* mDef;
        };
        
        std::ostream& operator<<(std::ostream& stream, const SSAValue& ssaVal);
        bool operator==(const SSAValue& lhsVal, const SSAValue& rhsVal);
        bool operator!=(const SSAValue& lhsVal, const SSAValue& rhsVal);
    }
}

#endif //SSA_VALUE__TOSTITOS
