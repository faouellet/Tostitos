#include "AST/ast.h"
#include "Parse/parser.h"
#include "Sema/symbolcollector.h"
#include "Sema/symboltable.h"
#include "Sema/typechecker.h"
#include "Utils/astprinter.h"

// TODO: include codegen

#include <iostream>

static void Compile(const std::string& programFile)
{
    TosLang::FrontEnd::Parser parser;
    auto programAST = parser.ParseProgram(programFile);
    if (programAST == nullptr)
        return;
 
    auto symbolTable = std::make_shared<TosLang::FrontEnd::SymbolTable>();
    TosLang::FrontEnd::SymbolCollector sCollector{ symbolTable };
    size_t errorCount = sCollector.Run(programAST);
    if (errorCount != 0)
        return;

    TosLang::FrontEnd::TypeChecker tChecker{ symbolTable };
    errorCount = tChecker.Run(programAST);
    if (errorCount != 0)
        return;
}

static void DumpAST(const std::string& programFile)
{
    TosLang::FrontEnd::Parser parser;
    auto programAST = parser.ParseProgram(programFile);
    if (programAST == nullptr)
        return;

    std::ostream& stream = std::cout;
    TosLang::Utils::ASTPrinter<std::ostream> printer(stream);
    printer.Run(programAST);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Correct usage: tc [options] <filename>"                               << std::endl
                  << "Options:"                                                             << std::endl
                  << "  -dump-ast                   Will output the program AST to stdout"  << std::endl;

    }
    else if (argc == 2)
    {
        Compile(argv[1]);
    }
    else
    {
        std::string arg = argv[1];
        if (arg == "-dump-ast")
        {
            DumpAST(argv[2]);
        }
    }
}