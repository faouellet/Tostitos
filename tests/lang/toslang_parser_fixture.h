#ifndef TOSLANG_PARSER_FIXTURE_H__TOSTITOS
#define TOSLANG_PARSER_FIXTURE_H__TOSTITOS

#include "AST/ast.h"
#include "AST/declarations.h"
#include "AST/expressions.h"
#include "AST/statements.h"
#include "Parse/parser.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace TosLang::FrontEnd;

/*
* \struct TosLangParserFixture
* \brief  Fixture used to test the parser of the TosLang compiler
*/
struct TosLangParserFixture
{
    /*
    * \fn    TosLangParserFixture
    * \brief Constructor. Redirect stderr to its internal buffer to help verify that the
    *        correct error messages are outputted during error case tests
    */
    TosLangParserFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
    }

    /*
    * \fn    ~TosLangFixture
    * \brief Destructor. Put stderr back in its original state
    */
    ~TosLangParserFixture()
    {
        std::cerr.rdbuf(oldBuffer);
        buffer.clear();
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

        auto& cNodes = pDecl->GetProgramDecls();
        BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

        return cNodes;
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

    std::unique_ptr<ASTNode> programAST;    /*!< Program abstract syntax tree */
    std::stringstream buffer;               /*!< Buffer in which to put error messages during testing */
    std::streambuf* oldBuffer;              /*!< Original stderr buffer */
};

#endif // TOSLANG_PARSER_FIXTURE_H__TOSTITOS
