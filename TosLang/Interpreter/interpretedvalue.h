#ifndef INTERPRETED_VALUE_H__TOSTITOS
#define INTERPRETED_VALUE_H__TOSTITOS

#include <string>

// TODO: Comments

namespace TosLang
{
    class IntepretedValue
    {
    public:
        enum class ValueType
        {
            BOOLEAN,
            INTEGER,
            STRING,
            UNKNOWN,
        };

    public:
        IntepretedValue() : mType{ ValueType::UNKNOWN } { }
        explicit IntepretedValue(bool val) : mType{ ValueType::BOOLEAN }, boolVal{ val } { }
        explicit IntepretedValue(int val) : mType{ ValueType::INTEGER }, intVal{ val } { }
        explicit IntepretedValue(const std::string& val): mType{ ValueType::STRING }, strVal{ val }
        {
            new (&strVal) std::string(val);
        }

        ~IntepretedValue()
        {
            if (mType == ValueType::STRING)
            {
                // Using statement is necessary to compile with Clang
                using std::string;
                strVal.~string();
            }
        }

    public:
        IntepretedValue& operator=(const IntepretedValue& val)
        {
            if (&val != this)
            {
                // Cleaning up dynamically allocated member
                if (mType == ValueType::STRING)
                {
                    // Using statement is necessary to compile with Clang
                    using std::string;
                    strVal.~string();
                }

                if (val.mType == ValueType::BOOLEAN)
                    boolVal = val.boolVal;
                if (val.mType == ValueType::INTEGER)
                    intVal = val.intVal;
                if (val.mType == ValueType::STRING)
                    new (&strVal) std::string(val.strVal);

                mType = val.mType;
            }

            return *this;
        }

    private:
        ValueType mType;
        union
        {
            bool boolVal;
            int intVal;
            std::string strVal;
        };
    };
}

#endif // INTERPRETED_VALUE_H__TOSTITOS
