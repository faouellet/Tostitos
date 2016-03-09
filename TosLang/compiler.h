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
    * \brief TODO
    */
    class Compiler
    {
    public:
        Compiler();
        ~Compiler();

    public:
        void Compile(const std::string& programFile);

    public:
        void DumpCFG(const std::string& programFile);
        void DumpAST(const std::string& programFile);

    private:
        std::unique_ptr<FrontEnd::ASTNode> ParseProgram(const std::string& programFile);

    private:
        std::shared_ptr<FrontEnd::SymbolTable> mSymTable;
        
        std::unique_ptr<FrontEnd::Parser> mParser;
        std::unique_ptr<FrontEnd::SymbolCollector> mSymCollector;
        std::unique_ptr<FrontEnd::ScopeChecker> mSChecker;
        std::unique_ptr<FrontEnd::TypeChecker> mTChecker;
        std::unique_ptr<BackEnd::InstructionSelector> mISel;
    };
}

#endif // COMPILER__TOSTITOS
