#ifndef CONSOLE_ERROR_FIXTURE_H__TOSTITOS
#define CONSOLE_ERROR_FIXTURE_H__TOSTITOS

#include "AST/ast.h"
#include "AST/declarations.h"
#include "AST/expressions.h"
#include "AST/statements.h"
#include "Parse/parser.h"
#include "Sema/symbolcollector.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace TosLang::FrontEnd;

struct TosLangFixture
{
    TosLangFixture()
    {
        oldBuffer = std::cerr.rdbuf();
        std::cerr.rdbuf(buffer.rdbuf());
    }

    ~TosLangFixture()
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

    const size_t GetProgramSymbolTable(const std::string& filename, std::shared_ptr<SymbolTable>& symTable)
    {
        auto& cNodes = GetProgramAST(filename);
        BOOST_REQUIRE_NE(cNodes.size(), 0);

        TosLang::FrontEnd::SymbolCollector sCollector{ symTable };
        return sCollector.Run(programAST);
    }

    std::unique_ptr<ASTNode> programAST;
    std::stringstream buffer;
    std::streambuf* oldBuffer;
};


#endif // CONSOLE_ERROR_FIXTURE_H__TOSTITOS
