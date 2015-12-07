#include "errorlogger.h"

#include "sourceloc.h"

#include <iostream>

using namespace TosLang::Utils;

std::unordered_map<ErrorLogger::ErrorType, std::string, ErrorLogger::ErrorTypeHash> ErrorLogger::mErrorMessages =
{
    // Binary Expr
    { ErrorType::MISSING_RHS,                  "ERROR: Missing right hand side in binary expression" },
    { ErrorType::WRONG_OPERATION,              "ERROR: Not an acceptable binary operation" },
    { ErrorType::WRONG_USE_OPERATION,          "ERROR: Not an acceptable use of a binary operation" },
    
    // Call
    { ErrorType::CALL_MISSING_PAREN,          "CALL ERROR: Function call is missing a closing parenthesis" },
    { ErrorType::CALL_ARG_ERROR,              "CALL ERROR: Incorrect function call arguments" },

    // File
    { ErrorType::WRONG_FILE_TYPE,              "FILE ERROR: Wrong file type" },
    { ErrorType::ERROR_OPENING_FILE,           "FILE ERROR: Problem opening the specified file" },

    // Function
    { ErrorType::FN_MISSING_TYPE,             "FUNCTION ERROR: Missing type from function declaration" },
    { ErrorType::FN_MISSING_IDENTIFIER,       "FUNCTION ERROR: The fn keyword should be followed by an identifier" },
    { ErrorType::FN_MISSING_PAREN,            "FUNCTION ERROR: Missing parenthesis in the function declaration" },
    { ErrorType::FN_MISSING_RETURN,           "FUNCTION ERROR: Missing return statement" },
    { ErrorType::FN_REDEFINITION,             "FUNCTION ERROR: Trying to redefine an already defined function" },

    // If
    { ErrorType::IF_MISSING_BODY,             "IF ERROR: Missing if body" },
    { ErrorType::IF_MISSING_COND,             "IF ERROR: Missing if condition" },

    // IO
    { ErrorType::PRINT_WRONG_MSG,             "PRINT ERROR: Wrong message for printing" },
    { ErrorType::SCAN_MISSING_INPUT_VAR,      "SCAN ERROR: Missing input variable" },
    { ErrorType::SCAN_WRONG_INPUT_VAR,        "SCAN ERROR: Incorrect input variable" },

    // Literal
    { ErrorType::NEW_LINE_IN_LITERAL,          "LITERAL ERROR: Newline in string literal" },
    { ErrorType::MISSING_CLOSING_QUOTE,        "LITERAL ERROR: Missing closing quote" },
    { ErrorType::NUMBER_BAD_SUFFIX,            "LITERAL ERROR: Bad suffix on number" },

    // Misc
    { ErrorType::EXPECTED_DECL,                "ERROR: Expected a declaration" },
    { ErrorType::MISSING_SEMI_COLON,           "ERROR: Expected a ;" },
    { ErrorType::UNCLOSED_ML_COMMENT,          "ERROR: Unclosed multiline comment" },

    // Syntax
    { ErrorType::SYNTAX_MISSING_LBRACE,        "SYNTAX ERROR: Expected '{'" },
    { ErrorType::SYNTAX_MISSING_RBRACE,        "SYNTAX ERROR: Expected '}'" },

    // Type
    { ErrorType::WRONG_LITERAL_TYPE,           "TYPE ERROR: Trying to instantiate variable with a literal of the wrong type" },
    { ErrorType::WRONG_VARIABLE_TYPE,          "TYPE ERROR: Type mismatch between variables" },

    // Var
    { ErrorType::VAR_MISSING_TYPE,             "VAR ERROR: Missing type from variable declaration" },
    { ErrorType::VAR_MISSING_COLON,            "VAR ERROR: Missing : between a variable and its type" },
    { ErrorType::VAR_MISSING_IDENTIFIER,       "VAR ERROR: The var keyword should be followed by an identifier" },
    { ErrorType::VAR_REDEFINITION,             "VAR ERROR: Trying to redefine an already defined variable" },
    { ErrorType::VAR_UNDECLARED_IDENTIFIER,    "VAR ERROR: Trying to assign an undeclared variable" },
    { ErrorType::VAR_VOID_TYPE,                "VAR ERROR: Trying to create a variable with void type" },

    // While
    { ErrorType::WHILE_MISSING_BODY,           "WHILE ERROR: Missing while body" },
    { ErrorType::WHILE_MISSING_COND,           "WHILE ERROR: Missing while condition" },
};

void ErrorLogger::PrintError(ErrorType eType)
{
    std::cerr << mErrorMessages[eType] << std::endl;
}

void ErrorLogger::PrintErrorAtLocation(ErrorType eType, const SourceLocation& srcLoc)
{
    std::cerr << mErrorMessages[eType] << " at line " << srcLoc.GetCurrentLine() << ", column " << srcLoc.GetCurrentColumn() << std::endl;
}
