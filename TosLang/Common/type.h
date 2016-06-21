#ifndef TYPE_H__TOSTITOS
#define TYPE_H__TOSTITOS

namespace TosLang
{
    namespace Common
    {
        enum class Type : int
        {
            // Default
            ERROR,

            // Builtins
            BOOL,
            NUMBER,
            STRING,
            VOID,

            // Arrays
            BOOL_ARRAY,
            NUMBER_ARRAY,
            STRING_ARRAY,

            UNKNOWN,
        };
    }
}

#endif // TYPE_H__TOSTITOS
