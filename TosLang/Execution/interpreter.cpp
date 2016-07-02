#include "interpreter.h"

#include "executor.h"

#include "../Parse/parser.h"
#include "../Sema/symbolcollector.h"
#include "../Sema/symboltable.h"
#include "../Sema/typechecker.h"

using namespace Execution;
using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

Interpreter::Interpreter()
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

Interpreter::~Interpreter() { }   // Required because of the forward declarations used in the header for our member pointers

bool Interpreter::Run(const std::string& programFile)
{
    // Let's start by building the AST
    auto programAST = mParser->ParseProgram(programFile);
    if (programAST == nullptr)
        return false;

    // Then we'll collect the program's symbols
    size_t errorCount = mSymCollector->Run(programAST);
    if (errorCount != 0)
        return false;

    // Making sure the program is well formed.
    // Also, since type checking rules dictate the choices made by overload resolution,
    // this will also ties functions and function calls together
    errorCount = mTChecker->Run(programAST, mSymTable);
    if (errorCount != 0)
        return false;

    // We're good to go. Let's jump into the 'main' function
    std::vector<Type> fnTypes{ Type::VOID };
    const ASTNode* mainNode = mSymTable->GetFunctionDecl({ fnTypes, "main" });
    if (mainNode == nullptr)
    {
        // TODO: Log error and add a unit test for it
        return false;
    }

    // Create a call stack for the main thread then push the global frame onto it
    // This frame will contains all global scope level informations that are available 
    // to all the functions in the program.
    // This make it the only frame that can be accessed by any other.
    CallStack cStack;
    cStack.EnterNewFrame(nullptr);

    Executor exec{};

    return true;
}
