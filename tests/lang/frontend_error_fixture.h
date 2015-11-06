#ifndef CONSOLE_ERROR_FIXTURE_H__TOSTITOS
#define CONSOLE_ERROR_FIXTURE_H__TOSTITOS

#include "Parse/parser.h"
#include "Parse/symboltable.h"
#include "AST/ast.h"
#include "AST/declarations.h"
#include "AST/expressions.h"
#include "AST/statements.h"

#include <boost/test/unit_test.hpp>

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

    const ChildrenNodes& GetProgramAST(const std::string& filename)
    {
        Parser parser(std::make_shared<SymbolTable>());
        programAST.reset(parser.ParseProgram(filename).release());
        BOOST_REQUIRE(programAST != nullptr);

        BOOST_REQUIRE(programAST->GetKind() == ASTNode::NodeKind::PROGRAM_DECL);
        ProgramDecl* pDecl = dynamic_cast<ProgramDecl*>(programAST.get());
        BOOST_REQUIRE(pDecl != nullptr);

        auto& cNodes = pDecl->GetProgramStmts();
        BOOST_REQUIRE(std::all_of(cNodes.begin(), cNodes.end(), [](const std::unique_ptr<ASTNode>& node) { return node != nullptr; }));

        return cNodes;
    }

    std::unique_ptr<ASTNode> programAST;
    std::shared_ptr<SymbolTable> symTab;
    std::stringstream buffer;
    std::streambuf* oldBuffer;
};


#endif // CONSOLE_ERROR_FIXTURE_H__TOSTITOS
