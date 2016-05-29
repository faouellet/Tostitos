#ifndef INTERPRETED_VALUE_H__TOSTITOS
#define INTERPRETED_VALUE_H__TOSTITOS

#include <cassert>
#include <ostream>
#include <string>

// TODO: Comments

namespace TosLang
{
    class InterpretedValue
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
        InterpretedValue() : mType{ ValueType::UNKNOWN } { }
        explicit InterpretedValue(bool val) : mType{ ValueType::BOOLEAN }, boolVal{ val } { }
        explicit InterpretedValue(int val) : mType{ ValueType::INTEGER }, intVal{ val } { }
        explicit InterpretedValue(const std::string& val): mType{ ValueType::STRING }, strVal{ val }
        {
            new (&strVal) std::string(val);
        }

        InterpretedValue(const InterpretedValue& iVal) : mType{ iVal.mType }
        {
            if (iVal.mType == ValueType::BOOLEAN)
                boolVal = iVal.boolVal;
            if (iVal.mType == ValueType::INTEGER)
                intVal = iVal.intVal;
            if (iVal.mType == ValueType::STRING)
                new (&strVal) std::string(iVal.strVal);
        }

        ~InterpretedValue()
        {
            if (mType == ValueType::STRING)
            {
                // Using statement is necessary to compile with Clang
                using std::string;
                strVal.~string();
            }
        }

    public:
        InterpretedValue& operator=(InterpretedValue val)
        {
            std::swap(val, *this);
            return *this;
        }

    public:
        friend std::ostream& operator<<(std::ostream& stream, const InterpretedValue& val)
        {
            switch (val.mType)
            {
            case InterpretedValue::ValueType::BOOLEAN:
                stream << val.boolVal;
                break;
            case InterpretedValue::ValueType::INTEGER:
                stream << val.intVal;
                break;
            case InterpretedValue::ValueType::STRING:
                stream << val.strVal;
                break;
            }

            return stream;
        }

    public:
        ValueType GetType() const { return mType; }

        bool GetBoolVal() const { assert(mType == ValueType::BOOLEAN); return boolVal; }
        int GetIntVal() const { assert(mType == ValueType::INTEGER); return intVal; }
        std::string GetStrVal() const { assert(mType == ValueType::STRING); return strVal; }

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
