#ifndef INTERPRETER_H__TOSTITOS
#define INTERPRETER_H__TOSTITOS

// TODO: Comments

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
}

namespace Execution
{
    class Interpreter
    {
    public:
        Interpreter();
        ~Interpreter();

        /*
        * \fn                   Run
        * \brief                Runs a TosLang program
        * \param programFile    Name (including path) of the .tos file to compile
        * \return               Has the program correctly terminated?
        */
        bool Run(const std::string& programFile);
        
    private:
        std::unique_ptr<TosLang::FrontEnd::ASTNode> mAST;                   /*!< Symbol table for a program */
      
        std::shared_ptr<TosLang::FrontEnd::SymbolTable> mSymTable;           /*!< Symbol table for a program */

        std::unique_ptr<TosLang::FrontEnd::Parser> mParser;                  /*!< Parser */
        std::unique_ptr<TosLang::FrontEnd::SymbolCollector> mSymCollector;   /*!< Symbol collector */
        std::unique_ptr<TosLang::FrontEnd::TypeChecker> mTChecker;           /*!< Type checker */
    };
}

#endif // INTERPRETER_H__TOSTITOS
