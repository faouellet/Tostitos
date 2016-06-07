#include "Execution/commandlineutil.h"
#include "Execution/compiler.h"
#include "Execution/interpreter.h"

using namespace Execution;

int main(int argc, char** argv)
{
    Compiler compiler;
    Interpreter interpreter;

    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    ExecutionCommand command = ParseCommandLine(args);
}