#include "Execution/commandlineutil.h"
#include "Execution/compiler.h"
#include "Execution/interpreter.h"

#include "AST/ast.h"    // TODO: Refactoring to remove this include

using namespace Execution;

int main(int argc, char** argv)
{
    Compiler compiler;
    Interpreter interpreter;

    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    ExecutionInfo info = ParseCommandLine(args);
    
    switch (info.command)
    {
    case Execution::ExecutionCommand::COMPILE_CHIP16:
        return 1;
    case Execution::ExecutionCommand::COMPILE_LLVM:
        return 1;
    case Execution::ExecutionCommand::DUMP_AST:
        compiler.DumpAST(info.programFile);
    case Execution::ExecutionCommand::DUMP_CFG:
        return 1;
    case Execution::ExecutionCommand::DUMP_LLVM:
        return 1;
    case Execution::ExecutionCommand::INTERPRET:
    {
        auto programAST = compiler.ParseProgram(info.programFile);
        if (programAST != nullptr)
        {
            auto symTable = compiler.GetSymbolTable(programAST);
            if (symTable != nullptr)
            {
                interpreter.Run(programAST, symTable);
            }
        }
    }
    default:
        return 1;
    }

    return 0;
}