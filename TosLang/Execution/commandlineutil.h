#ifndef COMMAND_LINE_UTIL_H__TOSTITOS
#define COMMAND_LINE_UTIL_H__TOSTITOS

#include <iostream>
#include <string>
#include <vector>

namespace Execution
{
    enum class ExecutionCommand
    {
        COMPILE_CHIP16,
        COMPILE_LLVM,
        DUMP_AST,
        DUMP_CFG,
        DUMP_LLVM,
        INTERPRET,
        UNKNOWN,
    };

    void ShowHelp()
    {
        std::cout << "Correct usage: tc [options] <filename>"                                       << std::endl
                  << "Options:"                                                                     << std::endl
                  << "  -compile=                   Compiles the program to the specified format"   << std::endl
                  << "      chip16                  Chip16 assembly"                                << std::endl
                  << "      llvm                    LLVM intermediate representation"               << std::endl
                  << "  -dump-ast                   Outputs the program AST to stdout"              << std::endl
                  << "  -dump-cfg                   Outputs the program CFG to stdout"              << std::endl
                  << "  -interpret                  Executes the program through an interpreter"    << std::endl
                  << "                              (Requires Tostitos to works)"                   << std::endl;
    }

    ExecutionCommand ParseCommandLine(const std::vector<std::string>& args)
    {
        if (args.size() > 3)
        {
            ShowHelp();
            return ExecutionCommand::UNKNOWN;
        }

        std::string arg = args[1];
        if (arg.find("-compile"))
        {
            if (arg.find("chip16"))
            {
                std::cout << "Not supported yet\n";
                //return ExecutionCommand::COMPILE_CHIP16;
                return ExecutionCommand::UNKNOWN;
            }
            else if (arg.find("llvm"))
            {
#ifdef USE_LLVM_BACKEND
                return ExecutionCommand::COMPILE_LLVM;
#else
                std::cout << "Requires building TosLang with LLVM backend\n";
                return ExecutionCommand::UNKNOWN;
#endif
            }
            else
            {
                std::cout << "Unknown format\n";
                return ExecutionCommand::UNKNOWN;
            }
        }
        else if (arg == "-dump-ast")
        {
            return ExecutionCommand::DUMP_AST;
        }
        //else if (arg == "-dump-cfg")
        //{
        //    return ExecutionCommand::DUMP_CFG;
        //}
        else if (arg == "-dump-llvm")
        {
#ifdef USE_LLVM_BACKEND
            return ExecutionCommand::DUMP_LLVM;
#else
            std::cout << "Requires building TosLang with LLVM backend\n";
            return ExecutionCommand::UNKNOWN;
#endif
        }
        else if (arg == "-interpret")
        {
            return ExecutionCommand::INTERPRET;
        }
        else
        {
            std::cout << "Unrecognized option\n";
            return ExecutionCommand::UNKNOWN;
        }
    }
}

#endif // COMMAND_LINE_UTIL_H__TOSTITOS
