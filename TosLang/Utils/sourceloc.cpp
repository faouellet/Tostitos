#include "sourceloc.h"

using namespace TosLang::Utils;

void SourceLocation::Init()
{
    mCurrentColumn = 1;
    mCurrentLine = 1;
}

void SourceLocation::Advance(bool nextLine)
{
    if (nextLine)
    {
        ++mCurrentLine;
        mCurrentColumn = 1;
    }
    else
    {
        ++mCurrentColumn;
    }
}
