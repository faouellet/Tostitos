#include "interpreter.h"

#include "../Parse/parser.h"
#include "../Sema/symbolcollector.h"
#include "../Sema/symboltable.h"
#include "../Sema/typechecker.h"

#include "../Utils/errorlogger.h"

#include "../../Tostitos/threading/threadutil.h"

using namespace Execution;
using namespace TosLang::FrontEnd;
using namespace TosLang::Common;

Interpreter::Interpreter() : mAST{ }
{
    mParser.reset(new Parser{});

    mSymTable.reset(new SymbolTable{});

    mSymCollector.reset(new SymbolCollector{ mSymTable });
    mTChecker.reset(new TypeChecker{});
}

Interpreter::~Interpreter() { }   // Required because of the forward declarations used in the header for our member pointers

// TODO: What about error handling?

bool Interpreter::Run(const std::string& programFile)
{
    // Let's start by building the AST
    mAST = mParser->ParseProgram(programFile);
    if (mAST == nullptr)
        return false;

    // Then we'll collect the program's symbols
    size_t errorCount = mSymCollector->Run(mAST);
    if (errorCount != 0)
        return false;

    // Making sure the program is well formed.
    // Also, since type checking rules dictate the choices made by overload resolution,
    // this will also ties functions and function calls together
    errorCount = mTChecker->Run(mAST, mSymTable);
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

    // TODO: Should this give a handle on the created thread?
    Threading::CreateThread(mAST.get(), mSymTable.get());
    
    return true;
}
