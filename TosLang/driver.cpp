#include "AST/ast.h"
#include "Parse/parser.h"
#include "Sema/symbolcollector.h"
#include "Sema/symboltable.h"
#include "Sema/typechecker.h"

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
    sCollector.Run(programAST);

    TosLang::FrontEnd::TypeChecker tChecker{ symbolTable };
    tChecker.Run(programAST);
    if (tChecker.GetErrorCount() != 0)
        return;
}

int main()
{
}