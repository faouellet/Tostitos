#ifndef INTERPRETER_H__TOSTITOS
#define INTERPRETER_H__TOSTITOS

#include "callstack.h"

#include <memory>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class Symbol;
        class SymbolTable;
    }
}

namespace Execution
{
    class Interpreter
    {
    public:
        Interpreter() : mCurrentScope{ 0 }, mCurrentNode{ nullptr }, mSymTable{ nullptr }, mCallStack{ } { }

    public:
        void Run(const std::unique_ptr<TosLang::FrontEnd::ASTNode>& root, 
                 const std::shared_ptr<TosLang::FrontEnd::SymbolTable>& symTab);
    
    protected:  // Declarations
        InterpretedValue HandleFunction(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleVarDecl(const TosLang::FrontEnd::ASTNode* node);
    
    protected:  // Expressions
        InterpretedValue HandleBinaryExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleBooleanExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleCallExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleIdentifierExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleNumberExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleStringExpr(const TosLang::FrontEnd::ASTNode* node);
    
    protected:  // Statements
        InterpretedValue HandleCompoundStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleIfStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandlePrintStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleReturnStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleScanStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleWhileStmt(const TosLang::FrontEnd::ASTNode* node);
    
    private:
        InterpretedValue DispatchNode(const TosLang::FrontEnd::ASTNode* node);
        const TosLang::FrontEnd::Symbol* GetSymbol(const TosLang::FrontEnd::ASTNode* node) const;

    private:
        size_t mCurrentScope;
        const TosLang::FrontEnd::ASTNode* mCurrentNode;
        std::shared_ptr<TosLang::FrontEnd::SymbolTable> mSymTable;
        CallStack mCallStack;
    };
}

#endif // INTERPRETER_H__TOSTITOS
