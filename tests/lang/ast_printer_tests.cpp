#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE ASTVisitorTests
#endif
#endif

#include "Parse/parser.h"
#include "Utils/astprinter.h"
#include "testutils.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

BOOST_AUTO_TEST_CASE( ASTPrinterTest )
{
    Parser parser;
    std::unique_ptr<ASTNode> rootNode = parser.ParseProgram("../sources/var/var_init_bool.tos");
    BOOST_REQUIRE(rootNode != nullptr);

    const std::string expectedFile = "../asts/var/var_init_bool.ast";
    const std::string testFile = "../asts/varinit_ast_tests.txt";

    {
        std::ofstream printStream(testFile);
        BOOST_REQUIRE(printStream.is_open());

        ASTPrinter<std::ofstream> printer{ printStream };
        printer.Run(rootNode);
        printStream.flush();
        printStream.close();
    }

    CompareFiles(testFile, expectedFile);
}
