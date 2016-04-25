#ifndef TOSLANG_SEMA_FIXTURE_H__TOSTITOS
#define TOSLANG_SEMA_FIXTURE_H__TOSTITOS

#include "AST/ast.h"
#include "Sema/overloadsolver.h"
#include "Sema/symbolcollector.h"
#include "Sema/typechecker.h"
#include "Utils/astreader.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace TosLang::FrontEnd;

/*
* \struct TosLangFixture
* \brief  Fixture used to test the semantic passes of the TosLang compiler
*/
struct TosLangSemaFixture
{
    /*
    * \fn    TosLangSemaFixture
    * \brief Constructor. Redirect stderr to its internal buffer to help verify that the 
    *        correct error messages are outputted during error case tests
    */
    TosLangSemaFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
    }

    /*
    * \fn    ~TosLangFixture
    * \brief Destructor. Put stderr back in its original state
    */
    ~TosLangSemaFixture()
    {
        std::cerr.rdbuf(oldBuffer);
        buffer.clear();
    }

    /*
    * \fn    GetErrorMessages
    * \brief Get the error message printed during a test's execution
    * return Error messages
    */
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

    /*
    * \fn               GetProgramSymbolTable
    * \brief            Parse a TosLang program and collect the symbols contained in the resulting AST
    * \param filename   Name of a file containing a TosLang AST
    * \param symTable   The symbol table to be filled
    * \return           The number of errors that happened during symbols collection
    */
    const size_t GetProgramSymbolTable(const std::string& filename, std::shared_ptr<SymbolTable>& symTable)
    {
        programAST = reader.Run(filename);
        BOOST_REQUIRE(programAST != nullptr);

        TosLang::FrontEnd::SymbolCollector sCollector{ symTable };
        return sCollector.Run(programAST);
    }

    /*
    * \fn               GetOverloadResolutionErrors
    * \brief            Parse a TosLang program and check the resulting AST for overload resolution errors
    * \param filename   Name of a file containing a TosLang AST
    * \return           The number of errors that happened during overload resolution
    */
    const size_t GetOverloadResolutionErrors(const std::string& filename)
    {
        auto symTable = std::make_shared<SymbolTable>();
        size_t errorCount = GetTypeErrors(filename);
        BOOST_REQUIRE_EQUAL(errorCount, 0);

        OverloadSolver oSolver;
        return oSolver.Run(programAST, symTable, tChecker.GetNodeTypeMapping());
    }

    /*
    * \fn               GetTypeErrors
    * \brief            Parse a TosLang program and check the resulting AST for type errors
    * \param filename   Name of a file containing a TosLang AST
    * \return           The number of errors that happened during type checking
    */
    const size_t GetTypeErrors(const std::string& filename)
    {
        auto symTable = std::make_shared<SymbolTable>();
        size_t errorCount = GetProgramSymbolTable(filename, symTable);
        BOOST_REQUIRE_EQUAL(errorCount, 0);

        return tChecker.Run(programAST, symTable);
    }

    std::unique_ptr<ASTNode> programAST;    /*!< Program abstract syntax tree */
    std::stringstream buffer;               /*!< Buffer in which to put error messages during testing */
    std::streambuf* oldBuffer;              /*!< Original stderr buffer */
    TosLang::Utils::ASTReader reader;       /*!< Use to acquire ASTs stored in files */
    TypeChecker tChecker;                   /*!< Type checker */
};


#endif // TOSLANG_SEMA_FIXTURE_H__TOSTITOS
