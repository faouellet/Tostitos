#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ISel
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "parser.h"
#include "instructionselector.h"

using namespace TosLang::FrontEnd;

BOOST_AUTO_TEST_CASE( VarInitSelectTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/varinit.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    TosLang::BackEnd::InstructionSelector iSel;
    std::vector<Instruction> program(iSel.Execute(rootNode));
}