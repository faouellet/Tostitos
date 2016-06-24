#ifndef TYPE_H__TOSTITOS
#define TYPE_H__TOSTITOS

namespace TosLang
{
    namespace Common
    {
        /*
        * \enum     Type
        * \brief    Possible types of a variable or identifier
        */
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

        /*
        * \fn           GetArrayVersion
        * \param type   Type to convert
        * \brief        Converts a scalar type to an array type
        * \return       Array version of the type given
        */
        Type GetArrayVersion(const Type type);
    }
}

#endif // TYPE_H__TOSTITOS
