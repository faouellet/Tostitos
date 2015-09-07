#include "lexer.h"

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
        return TOK_EOF;

    // Caching the current char because we will use it a lot
    const char currentChar = *mBufferIt;

	switch (currentChar)
	{
	case ':':
		++mBufferIt;
		return COLON;
	case ';':
		++mBufferIt;
		return SEMI_COLON;
	case '=':
		++mBufferIt;
		return EQUAL;
	case '+':
		++mBufferIt;
		return PLUS;
	case '-':
		++mBufferIt;
		return MINUS;
	case '*':
		++mBufferIt;
		return MULT;
	case '/':
		++mBufferIt;
		return DIVIDE;
	case '%':
		++mBufferIt;
		return MODULO;
	case '&':
		if (*(++mBufferIt) == '&')
		{
			++mBufferIt;
			return AND_BOOL;
		}
		else if (*mBufferIt == ' ')
		{
			return AND_INT;
		}
		else
		{
			return UNKNOWN;
		}
	case '|':
		if (*(++mBufferIt) == '|')
		{
			++mBufferIt;
			return OR_BOOL;
		}
		else if (*mBufferIt == ' ')
		{
			return OR_INT;
		}
		else
		{
			return UNKNOWN;
		}
	case '>':
		if (*(++mBufferIt) == '>')
		{
			++mBufferIt;
			return RIGHT_SHIFT;
		}
		else if (*mBufferIt == ' ')
		{
			return GREATER_THAN;
		}
		else
		{
			return UNKNOWN;
		}
	case '<':
		if (*(++mBufferIt) == '<')
		{
			++mBufferIt;
			return LEFT_SHIFT;
		}	
		else if (*mBufferIt == ' ')
		{
			return LESS_THAN;
		}
		else
		{
			return UNKNOWN;
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

			if (mCurrentStr == "var")
				return Lexer::Token::VAR;
			else if (mCurrentStr == "Int")
				return TYPE;
			else if (mCurrentStr == "Bool")
				return TYPE;
			else if (mCurrentStr == "True")
				return TRUE;
			else if (mCurrentStr == "False")
				return FALSE;
			else
				return IDENTIFIER;

		}
		// We have a numeric value
		else if (isdigit(currentChar))
		{
			std::string numberStr;
			do
			{
				numberStr += *mBufferIt;
			} while (isdigit(*(++mBufferIt)));

			mCurrentNumber = std::stoi(numberStr);

			return NUMBER;
		}
		return UNKNOWN;
	}
}
