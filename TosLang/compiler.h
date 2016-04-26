#ifndef COMPILER__TOSTITOS
#define COMPILER__TOSTITOS

#include <memory>
#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class Parser;
        class SymbolCollector;
        class SymbolTable;
        class TypeChecker;
    }

    namespace BackEnd
    {
        //class InstructionSelector;
        class LLVMGenerator;
    }

    /*
    * \class Compiler
    * \brief The TosLang compiler
    */
    class Compiler
    {
    public:
        /*
        * \fn Compiler
        * \brief Ctor
        */
        Compiler();

        /*
        * \fn ~Compiler
        * \brief Dtor
        */
        ~Compiler();

    public:
        /*
        * \fn                   Compile
        * \brief                Compiles a TosLang program to a MinChip16 binary
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void Compile(const std::string& programFile);

    public:
        /*
        * \fn                   DumpAST
        * \brief                Dumps the program's AST to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpAST(const std::string& programFile);

        /*
        * \fn                   DumpCFG
        * \brief                Dumps the program's CFG to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpCFG(const std::string& programFile);
        
#ifdef USE_LLVM_BACKEND
        /*
        * \fn                   DumpLLVMIR
        * \brief                Dumps the LLVM IR corresponding to the program to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpLLVMIR(const std::string& programFile);
#endif

    private:
        /*
        * \fn                   ParseProgram
        * \brief                Parse a TosLang program to produce an AST
        * \param programFile    Name (including path) of the .tos file to compile
        * \return               Root node of the program's AST
        */
        std::unique_ptr<FrontEnd::ASTNode> ParseProgram(const std::string& programFile);

    private:
        std::shared_ptr<FrontEnd::SymbolTable> mSymTable;           /*!< Symbol table for a program */
        
        std::unique_ptr<FrontEnd::Parser> mParser;                  /*!< Parser */
        std::unique_ptr<FrontEnd::SymbolCollector> mSymCollector;   /*!< Symbol collector */
        std::unique_ptr<FrontEnd::TypeChecker> mTChecker;           /*!< Type checker */
        //std::unique_ptr<BackEnd::InstructionSelector> mISel;        /*!< Instruction selector */

#ifdef USE_LLVM_BACKEND
        std::unique_ptr<BackEnd::LLVMGenerator> mLLVMGen;           /*!< LLVM IR Generator */
#endif
    };
}

#endif // COMPILER__TOSTITOS
