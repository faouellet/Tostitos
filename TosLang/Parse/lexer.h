#ifndef LEXER_H__TOSTITOS
#define LEXER_H__TOSTITOS

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
            enum Token
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

                // Misc
				ARROW,
				COMMA,
				COMMENT,
                COLON,
                IDENTIFIER,
				LEFT_BRACE,
				LEFT_BRACKET,
				LEFT_PAREN,
                NUMBER,
				QUOTE,
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
            Lexer() : mCurrentColumn{ 0 }, mCurrentLine{ 0 }, mCurrentNumber{ 0 } { };
            ~Lexer() = default;

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
            * \fn       GetCurrentColumn
            * \brief    Give the current number
            * \return   The current column
            */
            const int GetCurrentColumn() const { return mCurrentColumn; }

            /*
            * \fn       GetCurrentLine
            * \brief    Give the current number
            * \return   The current line
            */
            const int GetCurrentLine() const { return mCurrentLine; }

            /*
            * \fn       GetCurrentNumber
            * \brief    Give the current number
            * \return   The current number
            */
            const int GetCurrentNumber() const { return mCurrentNumber; }

            /*
            * \fn       GetCurrentStr
            * \brief    Give the current string
            * \return   The current string
            */
            const std::string& GetCurrentStr() const { return mCurrentStr; }

        private:
            unsigned mCurrentLine;              /*!< Current line in the file the lexer is at */
            unsigned mCurrentColumn;            /*!< Current column in the file the lexer is at */

            int mCurrentNumber;                 /*!< Current number in the lexer buffer */
            std::string mCurrentStr;            /*!< Current string in the lexer buffer */

            std::string mBuffer;                /*!< The lexer buffer */
            std::string::iterator mBufferIt;    /*!< Iterator of the lexer buffer */
        };
    }
}

#endif // LEXER_H__TOSTITOS
