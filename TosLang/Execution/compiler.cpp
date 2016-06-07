#include "Compiler.h"

//#include "CodeGen/instructionselector.h"
#include "../CFG/module.h"
#include "../Parse/parser.h"
#include "../Sema/symbolcollector.h"
#include "../Sema/symboltable.h"
#include "../Sema/typechecker.h"
#include "../SSA/cfgbuilder.h"
#include "../Utils/astprinter.h"

#ifdef USE_LLVM_BACKEND
#include "LLVMBackend/llvmgenerator.h"
#endif

#include <iostream>

using namespace Execution;
using namespace TosLang;
using namespace TosLang::BackEnd;
using namespace TosLang::FrontEnd;

Compiler::Compiler()
{
    mParser.reset(new Parser{});
    
    mSymTable.reset(new SymbolTable{});
    
    mSymCollector.reset(new SymbolCollector{ mSymTable });
    mTChecker.reset(new TypeChecker{});

    //mBuilder.reset(new CFGBuilder{});

    //mISel.reset(new BackEnd::InstructionSelector{});

#ifdef USE_LLVM_BACKEND
    mLLVNGen.reset(new BackEnd::LLVMGenerator{})
#endif
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

//void Compiler::DumpCFG(const std::string& programFile)
//{
//    auto programAST = ParseProgram(programFile);
//    if (programAST == nullptr)
//        return;
//
//    size_t errorCount = mSymCollector->Run(programAST);
//    if (errorCount != 0)
//        return;
//
//    std::unique_ptr<SSAModule> module = mBuilder->Run(programAST, mSymTable);
//    if (module == nullptr)
//        return;
//
//    module->Print(std::cout);
//}

#ifdef USE_LLVM_BACKEND
void Compiler::DumpLLVMIR(const std::string& programFile)
{
    auto programAST = ParseProgram(programFile);
    if (programAST == nullptr)
        return;

    size_t errorCount = mSymCollector->Run(programAST);
    if (errorCount != 0)
        return;

    auto llvmModule = mLLVMGen->Run(programAST, mSymTable);

    llvmModule->dump();
}
#endif

std::unique_ptr<ASTNode> Compiler::ParseProgram(const std::string & programFile)
{
    return mParser->ParseProgram(programFile);
}
