#ifndef ERROR_LOGGER_H__TOSTITOS
#define ERROR_LOGGER_H__TOSTITOS

#include <string>
#include <type_traits>
#include <unordered_map>

namespace TosLang
{
    namespace Utils
    {
        class ErrorLogger
        {
        public:
            enum class ErrorType : unsigned int
            {
                // File
                WRONG_FILE_TYPE,
                ERROR_OPENING_FILE,

                // Literal
                NEW_LINE_IN_LITERAL,
                NUMBER_BAD_SUFFIX,

                // Type
                WRONG_LITERAL_TYPE,
                WRONG_VARIABLE_TYPE,

                // Var
                VAR_MISSING_TYPE,
                VAR_MISSING_COLON,
                VAR_MISSING_IDENTIFIER,
                VAR_REDEFINITION,
                VAR_UNDECLARED_IDENTIFIER,

                // Misc
                MISSING_SEMI_COLON,
            };

        private:
            // TODO: This shouldn't be necessary with a fully compliant C++14 compiler
            struct ErrorTypeHash
            {
                template <typename T>
                std::size_t operator()(T t) const
                {
                    return static_cast<std::size_t>(t);
                }
            };

        public:
            ErrorLogger(const ErrorLogger&) = delete;
            ErrorLogger& operator=(const ErrorLogger&) = delete;
            ~ErrorLogger() = default;

        public:
            static void PrintError(ErrorType eType);
            static void PrintErrorAtLocation(ErrorType eType, unsigned line, unsigned column);

        private:
            ErrorLogger() = default;

        private:
            static std::unordered_map<ErrorType, std::string, ErrorTypeHash> mErrorMessages;
            static ErrorLogger mInstance;
        };
    }
}

#endif // ERROR_LOGGER_H__TOSTITOS
