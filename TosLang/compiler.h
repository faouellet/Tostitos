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
        class ScopeChecker;
        class TypeChecker;
    }

    namespace BackEnd
    {
        class InstructionSelector;
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
        * \fn                   DumpCFG
        * \brief                Dumps the program's CFG to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpCFG(const std::string& programFile);

        /*
        * \fn                   DumpAST
        * \brief                Dumps the program's AST to stdout
        * \param programFile    Name (including path) of the .tos file to compile
        */
        void DumpAST(const std::string& programFile);

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
        std::unique_ptr<FrontEnd::ScopeChecker> mSChecker;          /*!< Scope Checker */
        std::unique_ptr<FrontEnd::TypeChecker> mTChecker;           /*!< Type checker */
        std::unique_ptr<BackEnd::InstructionSelector> mISel;        /*!< Instruction selector */
    };
}

#endif // COMPILER__TOSTITOS
