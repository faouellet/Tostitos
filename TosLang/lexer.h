#ifndef LEXER_H__TOSTITOS
#define LEXER_H__TOSTITOS

#include <string>

/**
* \class Lexer
* \brief Part of the TosLang compiler used to generate a stream of tokens
*/
class Lexer
{
public:
    enum Token 
    { 
        // Keywords
        VAR,
        TYPE,

        // Boolean values
        FALSE,
        TRUE,

        // Operators
        EQUAL,

        // Misc
        COLON,
        IDENTIFIER,
        NUMBER,
        SEMI_COLON,

        // End of file
        TOK_EOF,

        // Unknown
        UNKNOWN
    };

public:
    Lexer() = default;
    ~Lexer() = default;

public:
    /*
    * \fn Init
    * \brief Initialize the lexer by acquiring the content of a TosLang file
    * \param filename Name of a file containing a TosLang program
    * \return Has the initialization been successful
    */
    bool Init(const std::string& filename);

    /*
    * \fn GetNextToken
    * \brief Get the next token in the program
    * \return A TosLang token
    */
    Token GetNextToken();

    /*
    * \fn GetCurrentNumber
    * \brief Give the current number
    * \return The current number
    */
    const int GetCurrentNumber() const { return mCurrentNumber; }

    /*
    * \fn GetCurrentStr
    * \brief Give the current string
    * \return The current string
    */
    const std::string& GetCurrentStr() const { return mCurrentStr; }

private:
    int mCurrentNumber;                 /*!< Current number in the lexer buffer */
    std::string mCurrentStr;            /*!< Current string in the lexer buffer */
    
    std::string mBuffer;                /*!< The lexer buffer */
    std::string::iterator mBufferIt;    /*!< Iterator of the lexer buffer */
};

#endif // LEXER_H__TOSTITOS
