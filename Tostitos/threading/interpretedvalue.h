#ifndef INTERPRETED_VALUE_H__TOSTITOS
#define INTERPRETED_VALUE_H__TOSTITOS

#include <cassert>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// TODO: Comments

namespace Threading
{
    namespace impl
    {
        class InterpretedValue
        {
        public:
            enum class ValueType
            {
                BOOLEAN,
                BOOLEAN_ARRAY,
                INTEGER,
                INTEGER_ARRAY,
                STRING,
                STRING_ARRAY,
                VOID,
                UNKNOWN,
            };

        public:
            InterpretedValue() : mType{ ValueType::UNKNOWN }, mIsReady{ false } { }
            explicit InterpretedValue(bool val) : mType{ ValueType::BOOLEAN }, boolVal{ new bool{ val } } { }
            explicit InterpretedValue(int val) : mType{ ValueType::INTEGER }, intVal{ new int { val } } { }
            explicit InterpretedValue(const std::string& val) : mType{ ValueType::STRING }, strVal{ new std::string{ val } } { }

            explicit InterpretedValue(const std::vector<bool>& vals)
                : mType{ ValueType::BOOLEAN_ARRAY }, boolArrayVal{ new std::vector<bool>{ vals } } { }
            explicit InterpretedValue(const std::vector<int>& vals)
                : mType{ ValueType::INTEGER_ARRAY }, intArrayVal{ new std::vector<int>{ vals } } { }
            explicit InterpretedValue(const std::vector<std::string>& vals)
                : mType{ ValueType::STRING_ARRAY }, strArrayVal{ new std::vector<std::string>{ vals } } { }

            InterpretedValue(const InterpretedValue& val) { AssignFrom(val); }

            ~InterpretedValue() { Destroy(); }

        public:
            static InterpretedValue CreateVoidValue()
            {
                InterpretedValue val;
                val.mType = ValueType::VOID;
                return val;
            }

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
                    stream << *val.boolVal;
                    break;
                case InterpretedValue::ValueType::BOOLEAN_ARRAY:
                    std::copy(val.boolArrayVal->begin(), val.boolArrayVal->end(), std::ostream_iterator<bool>(stream, ","));
                    break;
                case InterpretedValue::ValueType::INTEGER:
                    stream << *val.intVal;
                    break;
                case InterpretedValue::ValueType::INTEGER_ARRAY:
                    std::copy(val.intArrayVal->begin(), val.intArrayVal->end(), std::ostream_iterator<int>(stream, ","));
                    break;
                case InterpretedValue::ValueType::STRING:
                    stream << *val.strVal;
                    break;
                case InterpretedValue::ValueType::STRING_ARRAY:
                    std::copy(val.strArrayVal->begin(), val.strArrayVal->end(), std::ostream_iterator<std::string>(stream, ","));
                    break;
                default:
                    assert(false);  // Should never happen
                }

                return stream;
            }

        public:
            ValueType GetType() const { return mType; }
            bool IsReady() const { return mIsReady; }

            bool GetBoolVal() const { assert(mType == ValueType::BOOLEAN); return *boolVal; }
            int GetIntVal() const { assert(mType == ValueType::INTEGER); return *intVal; }
            const std::string& GetStrVal() const { assert(mType == ValueType::STRING); return *strVal; }

            const std::vector<bool>& GetBoolArrayVal() const { assert(mType == ValueType::BOOLEAN_ARRAY); return *boolArrayVal; }
            const std::vector<int>& GetIntArrayVal() const { assert(mType == ValueType::INTEGER_ARRAY); return *intArrayVal; }
            const std::vector<std::string>& GetStrArrayVal() const { assert(mType == ValueType::STRING_ARRAY); return *strArrayVal; }

        public:
            void SetReady() { mIsReady = true; }

        private:
            void Destroy()
            {
                switch (mType)
                {
                case ValueType::BOOLEAN:
                    boolVal.reset();
                    break;
                case ValueType::BOOLEAN_ARRAY:
                    boolArrayVal.reset();
                    break;
                case ValueType::INTEGER:
                    intVal.reset();
                    break;
                case ValueType::INTEGER_ARRAY:
                    intArrayVal.reset();
                    break;
                case ValueType::STRING:
                    strVal.reset();
                    break;
                case ValueType::STRING_ARRAY:
                    strArrayVal.reset();
                    break;
                case ValueType::VOID:
                    break;
                default:
                    assert(false);  // Should never happen
                }
            }

            void AssignFrom(const InterpretedValue& val)
            {
                switch (val.mType)
                {
                case ValueType::BOOLEAN:
                    boolVal.reset(new bool{ *val.boolVal });
                    break;
                case ValueType::BOOLEAN_ARRAY:
                    boolArrayVal.reset(new std::vector<bool>{ *val.boolArrayVal });
                    break;
                case ValueType::INTEGER:
                    intVal.reset(new int{ *val.intVal });
                    break;
                case ValueType::INTEGER_ARRAY:
                    intArrayVal.reset(new std::vector<int>{ *val.intArrayVal });
                    break;
                case ValueType::STRING:
                    strVal.reset(new std::string{ *val.strVal });
                    break;
                case ValueType::STRING_ARRAY:
                    strArrayVal.reset(new std::vector<std::string>{ *val.strArrayVal });
                    break;
                case ValueType::VOID:
                    break;
                default:
                    assert(false);  // Should never happen
                }

                mType = val.mType;
            }

        private:
            bool mIsReady;
            ValueType mType;
            union
            {
                std::unique_ptr<bool> boolVal;
                std::unique_ptr<int> intVal;
                std::unique_ptr<std::string> strVal;

                std::unique_ptr<std::vector<bool>> boolArrayVal;
                std::unique_ptr<std::vector<int>> intArrayVal;
                std::unique_ptr<std::vector<std::string>> strArrayVal;
            };
        };
    }   // namespace impl
}   // namespace Threading

#endif // INTERPRETED_VALUE_H__TOSTITOS
