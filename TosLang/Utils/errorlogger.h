#ifndef ERROR_LOGGER_H__TOSTITOS
#define ERROR_LOGGER_H__TOSTITOS

#include <string>
#include <type_traits>
#include <unordered_map>

namespace TosLang
{
    namespace Utils
    {
        class SourceLocation;

        /*
        * \class ErrorLogger
        * \brief Singleton class providing error logging services for the whole TosLang compiler
        */
        class ErrorLogger
        {
        public:
            /*
            * \enum     ErrorType
            * \brief    Every type of error that can be logged by the ErrorLogger
            */
            enum class ErrorType : unsigned int
            {
                // Binary Expr
                MISSING_RHS,
                WRONG_OPERATION,
                WRONG_USE_OPERATION,

                // Call
                CALL_ARG_ERROR,
                CALL_MISSING_PAREN,
                CALL_PARAM_TYPE_ERROR,
                CALL_RETURN_ERROR,
                CALL_NB_ARGS_ERROR,

                // File
                WRONG_FILE_TYPE,
                ERROR_OPENING_FILE,

                // Function
                FN_INTERNAL,
                FN_MISSING_ARROW,
                FN_MISSING_TYPE,
                FN_MISSING_IDENTIFIER,
                FN_MISSING_LEFT_PAREN,
                FN_MISSING_RIGHT_PAREN,
                FN_MISSING_RETURN,
                FN_MISSING_RETURN_TYPE,
                FN_REDEFINITION,
                FN_UNDECLARED,

                // If
                IF_MISSING_BODY,
                IF_MISSING_COND,

                // IO
                PRINT_WRONG_INPUT_TYPE,
                PRINT_WRONG_MSG,
                SCAN_MISSING_INPUT_VAR,
                SCAN_WRONG_INPUT_TYPE,

                // Literal
                NEW_LINE_IN_LITERAL,
                MISSING_CLOSING_QUOTE,
                NUMBER_BAD_SUFFIX,

                // Param
                PARAM_MISSING_NAME,
                PARAM_MISSING_COLON,
                PARAM_MISSING_TYPE,

                // Syntax
                SYNTAX_MISSING_LBRACE,
                SYNTAX_MISSING_RBRACE,

                // Type
                WRONG_BIN_EXPR_TYPE,
                WRONG_COND_EXPR_TYPE,
                WRONG_EXPR_TYPE,
                WRONG_LITERAL_TYPE,
                WRONG_RETURN_TYPE,
                WRONG_VARIABLE_TYPE,

                // Var
                PARAM_REDEFINITION,
                VAR_MISSING_TYPE,
                VAR_MISSING_COLON,
                VAR_MISSING_IDENTIFIER,
                VAR_REDEFINITION,
                VAR_UNDECLARED_IDENTIFIER,
                VAR_VOID_TYPE,

                // While
                WHILE_MISSING_BODY,
                WHILE_MISSING_COND,

                // Misc
                EXPECTED_DECL,
                MISSING_SEMI_COLON,
                UNCLOSED_ML_COMMENT,
            };

        private:
            // NOTE: This shouldn't be necessary with a fully compliant C++14 compiler
            /*
            * \struct   ErrorTypeHash
            * \brief    Functor providing a hash for the ErrorType enum
            */
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
            /*
            * \fn           PrintError
            * \param eType  Type of error to log
            * \brief        Logs the error of the type eType to the stderr
            */
            static void PrintError(ErrorType eType);
            
            /*
            * \fn           PrintErrorAtLocation
            * \param eType  Type of error to log
            * \param srcLoc Location in the source code where the error happened
            * \brief        Logs the error of the type eType with source location information to the stderr
            */
            static void PrintErrorAtLocation(ErrorType eType, const SourceLocation& srcLoc);

        private:
            ErrorLogger() = default;

        private:
            static std::unordered_map<ErrorType, std::string, ErrorTypeHash> mErrorMessages;    /*!< Maps the error types to the error messages */
            static ErrorLogger mInstance;                                                       /*!< Single instance of the error logger */
        };
    }
}

#endif // ERROR_LOGGER_H__TOSTITOS
