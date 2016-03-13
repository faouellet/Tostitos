#include "compiler.h"

#include "CodeGen/activationrecorder.h"
#include "CodeGen/instructionselector.h"
#include "CFG/module.h"
#include "Parse/parser.h"
#include "Sema/scopechecker.h"
#include "Sema/symbolcollector.h"
#include "Sema/symboltable.h"
#include "Sema/typechecker.h"
#include "Utils/astprinter.h"

#include <iostream>

using namespace TosLang;
using namespace TosLang::BackEnd;
using namespace TosLang::FrontEnd;

Compiler::Compiler()
{
    mParser.reset(new Parser{});
    
    mSymTable.reset(new SymbolTable{});

    mSymCollector.reset(new SymbolCollector{ mSymTable });
    mSChecker.reset(new ScopeChecker{});
    mTChecker.reset(new TypeChecker{});

    mRecorder.reset(new BackEnd::ActivationRecorder{});
    mISel.reset(new BackEnd::InstructionSelector{});
}

Compiler::~Compiler() { }   // Required because of the forward declarations used in the header for our member pointers

void Compiler::Compile(const std::string& programFile)
{
    auto programAST = ParseProgram(programFile);
    if (programAST == nullptr)
        return;

    size_t errorCount = mSymCollector->Run(programAST);
    if (errorCount != 0)
        return;

    errorCount = mTChecker->Run(programAST, mSymTable);
    if (errorCount != 0)
        return;
}

void Compiler::DumpAST(const std::string& programFile)
{
    auto programAST = ParseProgram(programFile);
    if (programAST == nullptr)
        return;

    std::ostream& stream = std::cout;
    TosLang::Utils::ASTPrinter<std::ostream> printer(stream);
    printer.Run(programAST);
}

void Compiler::DumpCFG(const std::string& programFile)
{
    auto programAST = ParseProgram(programFile);
    if (programAST == nullptr)
        return;

    size_t errorCount = mSymCollector->Run(programAST);
    if (errorCount != 0)
        return;

    FuncRecords records = mRecorder->Run(programAST, mSymTable);

    std::unique_ptr<Module> module = mISel->Run(programAST, mSymTable, std::move(records));
    if (module == nullptr)
        return;

    module->Print();
}

std::unique_ptr<ASTNode> TosLang::Compiler::ParseProgram(const std::string & programFile)
{
    return mParser->ParseProgram(programFile);
}
