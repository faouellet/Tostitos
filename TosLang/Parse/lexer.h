#ifndef LEXER_H__TOSTITOS
#define LEXER_H__TOSTITOS

#include "../Common/type.h"
#include "../Utils/sourceloc.h"

#include <locale>
#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        /**
        * \class Lexer
        * \brief Part of the TosLang compiler used to generate a stream of tokens
        */
        class Lexer
        {
        public:
            enum class Token : unsigned int
            {
                // Keywords
                FUNCTION,
                IF,
                PRINT,
                RETURN,
                SCAN,
                SPAWN,
                SYNC,
                TYPE,
                VAR,
                WHILE,

                // Boolean values
                FALSE,
                TRUE,

                // Operators
                AND_BOOL,
                AND_INT,
                DIVIDE,
                EQUAL,
                GREATER_THAN,
                LEFT_SHIFT,
                LESS_THAN,
                MINUS,
                MODULO,
                MULT,
                NOT,
                OR_BOOL,
                OR_INT,
                PLUS,
                RIGHT_SHIFT,
                OP_START = AND_BOOL,
                OP_END = RIGHT_SHIFT,

                // Misc
                ARROW,
                COMMA,
                COMMENT,
                COLON,
                IDENTIFIER,
                LEFT_BRACE,
                LEFT_BRACKET,
                LEFT_PAREN,
                ML_COMMENT,
                NUMBER,
                STRING_LITERAL,
                SEMI_COLON,
                RIGHT_BRACE,
                RIGHT_BRACKET,
                RIGHT_PAREN,

                // End of file
                TOK_EOF,

                // Unknown
                UNKNOWN
            };

        public:
            Lexer() : mCurrentType{ Common::Type::ERROR }, mCurrentNumber{ 0 }, mLoc{ "" } { };

        public:
            /*
            * \fn               Init
            * \brief            Initialize the lexer by acquiring the content of a TosLang file
            * \param filename   Name of a file containing a TosLang program
            * \return           Has the initialization been successful?
            */
            bool Init(const std::string& filename);

            /*
            * \fn       GetNextToken
            * \brief    Get the next token in the program
            * \return   A TosLang token
            */
            Token GetNextToken();
            
            /*
            * \fn       GetCurrentType
            * \brief    Gives the current type
            * \return   The current type
            */
            Common::Type GetCurrentType() const { return mCurrentType; }

            /*
            * \fn       GetCurrentLocation
            * \brief    Gives the current source location
            * \return   The current source location
            */
            const Utils::SourceLocation GetCurrentLocation() const { return mSrcLoc; }

            /*
            * \fn       GetCurrentNumber
            * \brief    Gives the current number
            * \return   The current number
            */
            int GetCurrentNumber() const { return mCurrentNumber; }

            /*
            * \fn       GetCurrentStr
            * \brief    Gives the current string
            * \return   The current string
            */
            const std::string& GetCurrentStr() const { return mCurrentStr; }

        private:
            Common::Type mCurrentType;          /*!< Current type recognized by the lexer */
            Utils::SourceLocation mSrcLoc;      /*!< Information about the current location of the lexer in the file */

            int mCurrentNumber;                 /*!< Current number in the lexer buffer */
            std::string mCurrentStr;            /*!< Current string in the lexer buffer */

            std::string mBuffer;                /*!< The lexer buffer */
            std::string::iterator mBufferIt;    /*!< Iterator of the lexer buffer */
            std::locale mLoc;                   /*!< The source locale */
        };
    }
}

#endif // LEXER_H__TOSTITOS
