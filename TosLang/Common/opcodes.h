#ifndef OPCODE_H__TOSTITOS
#define OPCODE_H__TOSTITOS

namespace TosLang
{
    namespace Common
    {
        enum class Operation : unsigned int
        {
            AND_BOOL,
            AND_INT,
            ASSIGNMENT,
            DIVIDE,
            GREATER_THAN,
            LEFT_SHIFT,
            LESS_THAN,
            MINUS,
            MODULO,
            MULT,
            NOT,
            OR_BOOL,
            OR_INT,
            PLUS,
            RIGHT_SHIFT,
            UNKNOWN_OP
        };
    }
}

#endif // OPCODE_H__TOSTITOS