#include "errorlogger.h"

#include <iostream>

using namespace TosLang::Utils;

std::unordered_map<ErrorLogger::ErrorType, std::string, ErrorLogger::ErrorTypeHash> ErrorLogger::mErrorMessages =
{
    // File
    { ErrorType::WRONG_FILE_TYPE,              "FILE ERROR: Wrong file type" },
    { ErrorType::ERROR_OPENING_FILE,           "FILE ERROR: Problem opening the specified file" },

    // Literal
    { ErrorType::NEW_LINE_IN_LITERAL,          "LITERAL ERROR: Newline in string literal" },
    { ErrorType::NUMBER_BAD_SUFFIX,            "LITERAL ERROR: Bad suffix on number" },

    // Misc
    { ErrorType::MISSING_SEMI_COLON,           "ERROR: Expected a ;" },
    { ErrorType::UNCLOSED_ML_COMMENT,          "ERROR: Unclosed multiline comment" },

    // Var
    { ErrorType::VAR_MISSING_TYPE,             "VAR ERROR: Missing type from variable declaration" },
    { ErrorType::VAR_MISSING_COLON,            "VAR ERROR: Missing : between a variable and its type" },
    { ErrorType::VAR_MISSING_IDENTIFIER,       "VAR ERROR: The var keyword should be followed by an identifier" },
    { ErrorType::VAR_REDEFINITION,             "VAR ERROR: Trying to redefine already define variable" },
    { ErrorType::VAR_UNDECLARED_IDENTIFIER,    "VAR ERROR: Trying to assign an undeclared variable" },

    // Type
    { ErrorType::WRONG_LITERAL_TYPE,           "TYPE ERROR: Trying to instantiate variable with a literal of the wrong type" },
    { ErrorType::WRONG_VARIABLE_TYPE,          "TYPE ERROR: Type mismatch between variables" }
};

void ErrorLogger::PrintError(ErrorType eType)
{
    std::cerr << mErrorMessages[eType] << std::endl;
}

void ErrorLogger::PrintErrorAtLocation(ErrorType eType, unsigned line, unsigned column)
{
    std::cerr << mErrorMessages[eType] << " at line " << line << ", column " << column << std::endl;
}
