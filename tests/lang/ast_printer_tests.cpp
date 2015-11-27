#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ASTVisitorTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "Utils/astprinter.h"
#include "Parse/parser.h"

#include <fstream>
#include <string>

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

BOOST_AUTO_TEST_CASE( ASTPrinterTest )
{
    Parser parser(std::make_shared<SymbolTable>());
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../inputs/var/var_init_bool.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    {
        std::ofstream printStream("../outputs/varinit_ast_tests.txt");
        BOOST_REQUIRE(printStream.is_open());

        ASTPrinter<std::ofstream> printer{ std::move(printStream) };
        printer.Print(rootNode);
        printStream.flush();
        printStream.close();
    }
    
    std::ifstream testStream("../outputs/varinit_ast_tests.txt");
    std::ifstream expectedStream("../outputs/varinit_ast.txt");

    std::string expectedStr;
    std::string testStr;
    while (!expectedStream.eof() && !testStream.eof())
    {
        expectedStream >> expectedStr;
        testStream >> testStr;

        BOOST_REQUIRE_EQUAL(expectedStr, testStr);
    }
}
