#ifndef CONSOLE_ERROR_FIXTURE_H__TOSTITOS
#define CONSOLE_ERROR_FIXTURE_H__TOSTITOS

#include "Parse\symboltable.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace TosLang::FrontEnd;

struct FrontEndErrorFixture
{
    FrontEndErrorFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
        symTab = std::make_shared<SymbolTable>();
    }

    ~FrontEndErrorFixture()
    {
        std::cerr.rdbuf(oldBuffer);
        buffer.clear();
    }

    std::vector<std::string> GetErrorMessages()
    {
        std::vector<std::string> errorMessages;
        std::string message;
        while (std::getline(buffer, message))
        {
            errorMessages.push_back(message);
        }

        return errorMessages;
    }

    std::shared_ptr<SymbolTable> symTab;
    std::stringstream buffer;
    std::streambuf* oldBuffer;
};


#endif // CONSOLE_ERROR_FIXTURE_H__TOSTITOS
