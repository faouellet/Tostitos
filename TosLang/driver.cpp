#include "compiler.h"

#include <iostream>

int main(int argc, char** argv)
{
    TosLang::Compiler compiler;

    if (argc < 2)
    {
        std::cout << "Correct usage: tc [options] <filename>"                               << std::endl
                  << "Options:"                                                             << std::endl
                  << "  -dump-ast                   Will output the program AST to stdout"  << std::endl
                  << "  -dump-cfg                   Will output the program CFG to stdout"  << std::endl;

    }
    else if (argc == 2)
    {
        compiler.Compile(argv[1]);
    }
    else
    {
        std::string arg = argv[1];
        if (arg == "-dump-ast")
        {
            compiler.DumpAST(argv[2]);
        }
        else if (arg == "-dump-cfg")
        {
            compiler.DumpCFG(argv[2]);
        }
    }
}