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
        return true;
    }
    return false;
}

Lexer::Token Lexer::GetNextToken()
{
    // End of file, we're done
    if (mBufferIt == mBuffer.end())
        return TOK_EOF;
    
    // Skipping any whitespaces
    mBufferIt = std::find_if(mBufferIt, mBuffer.end(), [](const char c) { return !isspace(c); });
    if (mBufferIt == mBuffer.end())
        return TOK_EOF;

    // Caching the current char because we will use it a lot
    const char currentChar = *mBufferIt;

    // We have and identifier or a keyword
    if (isalpha(currentChar))
    {
        mCurrentStr = currentChar;
        while (isalnum(*(++mBufferIt)))
            mCurrentStr += *mBufferIt;

        if (mCurrentStr == "var")
            return Lexer::Token::VAR;
        else if (mCurrentStr == "Int")
            return INT;
        else if (mCurrentStr == "Bool")
            return BOOL;
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
