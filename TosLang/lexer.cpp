#include "lexer.h"

#include <algorithm>
#include <fstream>
#include <iterator>

bool Lexer::Init(const std::string& filename)
{
    std::ifstream stream(filename);
    if (stream.is_open())
    {
        mBuffer.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        mBufferIt = mBuffer.begin();
        mCurrentColumn = 0;
        mCurrentLine = 0;
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
            mCurrentColumn = 0;
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

    // We have and identifier or a keyword
    if (isalpha(currentChar))
    {
        mCurrentStr = currentChar;
        while (++mBufferIt != mBuffer.end() && isalnum(*mBufferIt))
            mCurrentStr += *mBufferIt;

        if (mCurrentStr == "var")
            return Lexer::Token::VAR;
        else if (mCurrentStr == "Int")
        {
            mCurrentType = INT;
            return TYPE;
        }
        else if (mCurrentStr == "Bool")
        {
            mCurrentType = BOOL;
            return TYPE;
        }
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
    else if (currentChar == ':')
    {
        ++mBufferIt;
        return COLON;
    }
    else if (currentChar == ';')
    {
        ++mBufferIt;
        return SEMI_COLON;
    }
    else if (currentChar == '=')
    {
        ++mBufferIt;
        return EQUAL;
    }
    else
    {
        return UNKNOWN;
    }
}
