#ifndef INTERPRETED_VALUE_H__TOSTITOS
#define INTERPRETED_VALUE_H__TOSTITOS

#include <cassert>
#include <ostream>
#include <string>

// TODO: Comments

namespace Execution
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

        InterpretedValue(const InterpretedValue& val) { AssignFrom(val); }

        ~InterpretedValue() { Destroy(); }

    public:
        InterpretedValue& operator=(const InterpretedValue& val)
        {
            if (&val != this)
            {
                Destroy();
                AssignFrom(val);
            }

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
        void Destroy()
        {
            if (mType == ValueType::STRING)
            {
                // Using statement is necessary to compile with Clang
                using std::string;
                strVal.~string();
            }
        }

        void AssignFrom(const InterpretedValue& val)
        {
            switch (val.mType)
            {
            case ValueType::BOOLEAN:
                boolVal = val.boolVal;
                break;
            case ValueType::INTEGER:
                intVal = val.intVal;
                break;
            case ValueType::STRING:
                new (&strVal) std::string(val.strVal);
                break;
            default:
                assert(false);  // Should never happen
            }

            mType = val.mType;
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
