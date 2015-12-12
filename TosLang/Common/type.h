#ifndef TYPE_H__TOSTITOS
#define TYPE_H__TOSTITOS

namespace TosLang
{
    namespace Common
    {
        enum class Type : int
        {
            // Builtins
            BOOL,
            INT,
            STRING,
            VOID,

            FUNCTION,

            // Arrays

            // Default
            UNKNOWN,
        };
    }
}

#endif // TYPE_H__TOSTITOS
