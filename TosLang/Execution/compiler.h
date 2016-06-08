#ifndef COMPILER__TOSTITOS
#define COMPILER__TOSTITOS

#include <memory>
#include <string>

namespace TosLang
{
    class Interpreter;

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
        //class CFGBuilder;
        //class InstructionSelector;
        class LLVMGenerator;
    }
}

namespace Execution
{
    /*
    * \class Compiler
    * \brief The TosLang driver
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
        //void DumpCFG(const std::string& programFile);
        
#ifdef USE_LLVM_BACKEND
        /*
        * \fn                   DumpLLVMIR
        * \brief                Dumps the LLVM IR corresponding to the program to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpLLVMIR(const std::string& programFile);
#endif

    public:
        /*
        * \fn                   ParseProgram
        * \brief                Parse a TosLang program to produce an AST
        * \param programFile    Name (including path) of the .tos file to compile
        * \return               Root node of the program's AST
        */
        std::unique_ptr<TosLang::FrontEnd::ASTNode> ParseProgram(const std::string& programFile);

        const TosLang::FrontEnd::SymbolTable* GetSymbolTable(const std::unique_ptr<TosLang::FrontEnd::ASTNode>& root);

    private:
        std::shared_ptr<TosLang::FrontEnd::SymbolTable> mSymTable;           /*!< Symbol table for a program */

        std::unique_ptr<TosLang::FrontEnd::Parser> mParser;                  /*!< Parser */
        std::unique_ptr<TosLang::FrontEnd::SymbolCollector> mSymCollector;   /*!< Symbol collector */
        std::unique_ptr<TosLang::FrontEnd::TypeChecker> mTChecker;           /*!< Type checker */
        //std::unique_ptr<BackEnd::CFGBuilder> mBuilder;              /*!< CFG Builder */
        //std::unique_ptr<BackEnd::InstructionSelector> mISel;        /*!< Instruction selector */

#ifdef USE_LLVM_BACKEND
        std::unique_ptr<BackEnd::LLVMGenerator> mLLVMGen;           /*!< LLVM IR Generator */
#endif
    };
}

#endif // COMPILER__TOSTITOS
