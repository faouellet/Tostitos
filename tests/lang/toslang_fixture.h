#ifndef CONSOLE_ERROR_FIXTURE_H__TOSTITOS
#define CONSOLE_ERROR_FIXTURE_H__TOSTITOS

#include "AST/ast.h"
#include "AST/declarations.h"
#include "AST/expressions.h"
#include "AST/statements.h"
#include "Parse/parser.h"
#include "Sema/symbolcollector.h"
#include "Sema/typechecker.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace TosLang::FrontEnd;

/*
* \struct TosLangFixture
* \brief  Fixture used to test the TosLang compiler
*/
struct TosLangFixture
{
    /*
    * \fn    TosLangFixture
    * \brief Constructor. Redirect stderr to its internal buffer to help verify that the 
    *        correct error messages are outputted during error case tests
    */
    TosLangFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
    }

    /*
    * \fn    ~TosLangFixture
    * \brief Destructor. Put stderr back in its original state
    */
    ~TosLangFixture()
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
    * \fn               GetProgramAST
    * \brief            Parse a TosLang program to construct its AST
    * \param filename   Name of a file containing a TosLang program
    * \return           The nodes making up the program's AST
    */
    const ChildrenNodes& GetProgramAST(const std::string& filename)
    {
        Parser parser;
        programAST.reset(parser.ParseProgram(filename).release());
        BOOST_REQUIRE(programAST != nullptr);

        BOOST_REQUIRE(programAST->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
        ProgramDecl* pDecl = dynamic_cast<ProgramDecl*>(programAST.get());
        BOOST_REQUIRE(pDecl != nullptr);

        auto& cNodes = pDecl->GetProgramStmts();
        BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

        return cNodes;
    }

    /*
    * \fn               GetProgramSymbolTable
    * \brief            Parse a TosLang program and collect the symbols contained in the resulting AST
    * \param filename   Name of a file containing a TosLang program
    * \param symTable   The symbol table to be filled
    * \return           The number of errors that happened during symbols collection
    */
    const size_t GetProgramSymbolTable(const std::string& filename, std::shared_ptr<SymbolTable>& symTable)
    {
        auto& cNodes = GetProgramAST(filename);
        BOOST_REQUIRE_NE(cNodes.size(), 0);

        TosLang::FrontEnd::SymbolCollector sCollector{ symTable };
        return sCollector.Run(programAST);
    }

    /*
    * \fn               GetTypeErrors
    * \brief            Parse a TosLang program and check the resulting AST for type errors
    * \param filename   Name of a file containing a TosLang program
    * \return           The number of errors that happened during type checking
    */
    const size_t GetTypeErrors(const std::string& filename)
    {
        auto symTable = std::make_shared<SymbolTable>();
        size_t errorCount = GetProgramSymbolTable(filename, symTable);
        BOOST_REQUIRE_EQUAL(errorCount, 0);

        TypeChecker tChecker(symTable);
        return tChecker.Run(programAST);
    }

    std::unique_ptr<ASTNode> programAST;    /*!< Program abstract syntax tree */
    std::stringstream buffer;               /*!< Buffer in which to put error messages during testing */
    std::streambuf* oldBuffer;              /*!< Original stderr buffer */
};


#endif // CONSOLE_ERROR_FIXTURE_H__TOSTITOS
