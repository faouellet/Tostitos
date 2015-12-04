#ifndef TYPE_H__TOSTITOS
#define TYPE_H__TOSTITOS

namespace TosLang
{
    namespace Common
    {
        enum class Type
        {
            // Builtins
            BOOL,
            INT,
            STRING,

            // Functions
            BOOL_FUNCTION,
            INT_FUNCTION,
            STRING_FUNCTION,
            VOID_FUNCTION,

            // Arrays

            // Default
            UNKNOWN,
        };
    }
}

#endif // TYPE_H__TOSTITOS
