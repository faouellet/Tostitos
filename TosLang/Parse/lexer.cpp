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
        return TOK_EOF;

    // Caching the current char because we will use it a lot
    const char currentChar = *mBufferIt;

	switch (currentChar)
	{
		// TODO: Should the balance of " be checked here?
	case ',':
		++mBufferIt;
		return COMMA;
	case ':':
		++mBufferIt;
		return COLON;
	case ';':
		++mBufferIt;
		return SEMI_COLON;
	case '{':
		++mBufferIt;
		return LEFT_BRACE;
	case '[':
		++mBufferIt;
		return LEFT_BRACKET;
	case '(':
		++mBufferIt;
		return LEFT_PAREN;
	case '}':
		++mBufferIt;
		return RIGHT_BRACE;
	case ']':
		++mBufferIt;
		return RIGHT_BRACKET;
	case ')':
		++mBufferIt;
		return RIGHT_PAREN;
	case '=':
		++mBufferIt;
		return EQUAL;
	case '+':
		++mBufferIt;
		return PLUS;
	case '*':
		++mBufferIt;
		return MULT;
	case '%':
		++mBufferIt;
		return MODULO;
	case '!':
		++mBufferIt;
		return NOT;
	case '-':
		if (*(++mBufferIt) == '>')
		{
			++mBufferIt;
			return ARROW;
		}
		else
		{
			return MINUS;
		}
	case '/':
		if (*(++mBufferIt) == '/')
		{
			// A comment covers a whole line
			while (mBufferIt != mBuffer.end() && *mBufferIt != '\n')
				++mBufferIt;
			return COMMENT;
		}
		else
		{
			return DIVIDE;
		}
	case '&':
		if (*(++mBufferIt) == '&')
		{
			++mBufferIt;
			return AND_BOOL;
		}
		else
		{
			return AND_INT;
		}
	case '|':
		if (*(++mBufferIt) == '|')
		{
			++mBufferIt;
			return OR_BOOL;
		}
		else
		{
			return OR_INT;
		}
	case '>':
		if (*(++mBufferIt) == '>')
		{
			++mBufferIt;
			return RIGHT_SHIFT;
		}
		else
		{
			return GREATER_THAN;
		}
	case '<':
		if (*(++mBufferIt) == '<')
		{
			++mBufferIt;
			return LEFT_SHIFT;
		}	
		else
		{
			return LESS_THAN;
		}
	case '\"':
		mCurrentStr.clear();
        while ((++mBufferIt != mBuffer.end()) && (*mBufferIt != '\n') && (*mBufferIt != '\"'))
            mCurrentStr += *mBufferIt;

		if ((mBufferIt == mBuffer.end()) || (*mBufferIt == '\n'))
		{
            Utils::ErrorLogger::PrintErrorAtLocation(Utils::ErrorLogger::NEW_LINE_IN_LITERAL, mCurrentLine, mCurrentColumn);
			return UNKNOWN;	// TODO: This needs to be logged by the error logger
		}
		else
		{
			++mBufferIt;
			return STRING_LITERAL;
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
				return FUNCTION;
			else if (mCurrentStr == "print")
				return PRINT;
			else if (mCurrentStr == "scan")
				return SCAN;
			else if (mCurrentStr == "String")
				return TYPE;
			else if (mCurrentStr == "var")
				return VAR;
			else if (mCurrentStr == "Void")
				return TYPE;
			else if (mCurrentStr == "Int")
				return TYPE;
			else if (mCurrentStr == "Bool")
				return TYPE;
			else if (mCurrentStr == "True")
				return TRUE;
			else if (mCurrentStr == "False")
				return FALSE;
			else if (mCurrentStr == "if")
				return IF;
			else if (mCurrentStr == "while")
				return WHILE;
			else if (mCurrentStr == "spawn")
				return SPAWN;
			else if (mCurrentStr == "sync")
				return SYNC;
			else if (mCurrentStr == "return")
				return RETURN;
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
