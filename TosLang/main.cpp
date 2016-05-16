#include "toslangdriver.h"

#include <iostream>

int main(int argc, char** argv)
{
    TosLang::TosLangDriver driver;

    if (argc < 3)
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
    else
    {
        std::string arg = argv[1];
        if (arg.find("-compile"))
        {
            if (arg.find("chip16"))
            {
                std::cout << "Not supported yet\n";
            }
            else if (arg.find("llvm"))
            {
#ifdef USE_LLVM_BACKEND
                //
#else
                std::cout << "Requires building TosLang with LLVM backend\n";
#endif

            }
            else
            {
                std::cout << "Unknown format\n";
            }
        }
        else if (arg == "-dump-ast")
        {
            driver.DumpAST(argv[2]);
        }
        //else if (arg == "-dump-cfg")
        //{
        //    compiler.DumpCFG(argv[2]);
        //}
        else if (arg == "-dump-llvm")
        {
#ifdef USE_LLVM_BACKEND
            compiler.DumpLLVMIR(argv[2]);
#else
            std::cout << "Requires building TosLang with LLVM backend\n";
#endif
        }
        else if (arg == "-interpret")
        {
            std::cout << "Not supported yet\n";
        }
        else
        {
            std::cout << "Unrecognized option\n";
        }
    }
}