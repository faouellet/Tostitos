#include "lexer.h"

#include "../Utils/errorlogger.h"

#include <algorithm>
#include <fstream>
#include <iterator>

using namespace TosLang::FrontEnd;

bool Lexer::Init(const std::string& filename)
{
    std::ifstream stream(filename);
    if (stream.is_open())
    {
        mBuffer.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        mBufferIt = mBuffer.begin();
        mCurrentColumn = 1;
        mCurrentLine = 1;
        return true;
    }
    return false;
}

Lexer::Token Lexer::GetNextToken()
{
    // Skipping any whitespaces
    while (mBufferIt != mBuffer.end() && isspace(*mBufferIt))
    {
        if (*mBufferIt == '\n')
        {
            ++mCurrentLine;
            mCurrentColumn = 1;
        }
        else
        {
            ++mCurrentColumn;
        }

        ++mBufferIt;
    }

    // End of file, we're done
    if (mBufferIt == mBuffer.end())
        return Token::TOK_EOF;

    // Caching the current char because we will use it a lot
    const char currentChar = *mBufferIt;

	switch (currentChar)
	{
		// TODO: Should the balance of " be checked here?
	case ',':
		++mBufferIt;
		return Token::COMMA;
	case ':':
		++mBufferIt;
		return Token::COLON;
	case ';':
		++mBufferIt;
		return Token::SEMI_COLON;
	case '{':
		++mBufferIt;
		return Token::LEFT_BRACE;
	case '[':
		++mBufferIt;
		return Token::LEFT_BRACKET;
	case '(':
		++mBufferIt;
		return Token::LEFT_PAREN;
	case '}':
		++mBufferIt;
		return Token::RIGHT_BRACE;
	case ']':
		++mBufferIt;
		return Token::RIGHT_BRACKET;
	case ')':
		++mBufferIt;
		return Token::RIGHT_PAREN;
	case '=':
		++mBufferIt;
		return Token::EQUAL;
	case '+':
		++mBufferIt;
		return Token::PLUS;
	case '*':
		++mBufferIt;
		return Token::MULT;
	case '%':
		++mBufferIt;
		return Token::MODULO;
	case '!':
		++mBufferIt;
		return Token::NOT;
	case '-':
		if (*(++mBufferIt) == '>')
		{
			++mBufferIt;
			return Token::ARROW;
		}
		else
		{
			return Token::MINUS;
		}
	case '/':
		if (*(++mBufferIt) == '/')
		{
			// A comment covers a whole line
			while (mBufferIt != mBuffer.end() && *mBufferIt != '\n')
				++mBufferIt;
			return Token::COMMENT;
		}
		else
		{
			return Token::DIVIDE;
		}
	case '&':
		if (*(++mBufferIt) == '&')
		{
			++mBufferIt;
			return Token::AND_BOOL;
		}
		else
		{
			return Token::AND_INT;
		}
	case '|':
		if (*(++mBufferIt) == '|')
		{
			++mBufferIt;
			return Token::OR_BOOL;
		}
		else
		{
			return Token::OR_INT;
		}
	case '>':
		if (*(++mBufferIt) == '>')
		{
			++mBufferIt;
			return Token::RIGHT_SHIFT;
		}
		else
		{
			return Token::GREATER_THAN;
		}
	case '<':
		if (*(++mBufferIt) == '<')
		{
			++mBufferIt;
			return Token::LEFT_SHIFT;
		}	
		else
		{
			return Token::LESS_THAN;
		}
	case '\"':
		mCurrentStr.clear();
        while ((++mBufferIt != mBuffer.end()) && (*mBufferIt != '\n') && (*mBufferIt != '\"'))
            mCurrentStr += *mBufferIt;

		if ((mBufferIt == mBuffer.end()) || (*mBufferIt == '\n'))
		{
            Utils::ErrorLogger::PrintErrorAtLocation(Utils::ErrorLogger::ErrorType::NEW_LINE_IN_LITERAL, mCurrentLine, mCurrentColumn);
			return Token::UNKNOWN;
		}
		else
		{
			++mBufferIt;
			return Token::STRING_LITERAL;
		}
	default:
		// We have and identifier or a keyword
		if (isalpha(currentChar))
		{
			mCurrentStr = currentChar;
			while (++mBufferIt != mBuffer.end() && isalnum(*mBufferIt))
			{
				mCurrentStr += *mBufferIt;
				++mCurrentColumn;
			}

			if (mCurrentStr == "fn")
				return Token::FUNCTION;
			else if (mCurrentStr == "print")
				return Token::PRINT;
			else if (mCurrentStr == "scan")
				return Token::SCAN;
			else if (mCurrentStr == "String")
				return Token::TYPE;
			else if (mCurrentStr == "var")
				return Token::VAR;
			else if (mCurrentStr == "Void")
				return Token::TYPE;
			else if (mCurrentStr == "Int")
				return Token::TYPE;
			else if (mCurrentStr == "Bool")
				return Token::TYPE;
			else if (mCurrentStr == "True")
				return Token::TRUE;
			else if (mCurrentStr == "False")
				return Token::FALSE;
			else if (mCurrentStr == "if")
				return Token::IF;
			else if (mCurrentStr == "while")
				return Token::WHILE;
			else if (mCurrentStr == "spawn")
				return Token::SPAWN;
			else if (mCurrentStr == "sync")
				return Token::SYNC;
			else if (mCurrentStr == "return")
				return Token::RETURN;
			else
				return Token::IDENTIFIER;
		}
		// We have a numeric value
		else if (isdigit(currentChar))
		{
			std::string numberStr;
			do
			{
				numberStr += *mBufferIt;
			} while (isdigit(*(++mBufferIt)));

            if (isalpha(*mBufferIt))
            {
                // Error: letter in a number
                Utils::ErrorLogger::PrintErrorAtLocation(Utils::ErrorLogger::ErrorType::NUMBER_BAD_SUFFIX, mCurrentLine, mCurrentColumn);
                // Advance to the next statement
                while ((mBufferIt != mBuffer.end()) && (*mBufferIt != ';'))
                    ++mBufferIt;

                if (mBufferIt != mBuffer.end())
                    ++mBufferIt;

                return Token::UNKNOWN;
            }
            else
            {
                mCurrentNumber = std::stoi(numberStr);
                return Token::NUMBER;
            }
		}

		return Token::UNKNOWN;
	}
}
