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

    class Interpreter
    {
    public:
        Interpreter() { }

    public:
        void Run(const std::unique_ptr<FrontEnd::ASTNode>& root, const std::shared_ptr<FrontEnd::SymbolTable>& symTab);
    
    protected:  // Declarations
        InterpretedValue HandleFunction(const FrontEnd::ASTNode* node);
        InterpretedValue HandleVarDecl(const FrontEnd::ASTNode* node);
    
    protected:  // Expressions
        InterpretedValue HandleBinaryExpr(const FrontEnd::ASTNode* node);
        InterpretedValue HandleBooleanExpr(const FrontEnd::ASTNode* node);
        InterpretedValue HandleCallExpr(const FrontEnd::ASTNode* node);
        InterpretedValue HandleIdentifierExpr(const FrontEnd::ASTNode* node);
        InterpretedValue HandleNumberExpr(const FrontEnd::ASTNode* node);
        InterpretedValue HandleStringExpr(const FrontEnd::ASTNode* node);
    
    protected:  // Statements
        InterpretedValue HandleCompoundStmt(const FrontEnd::ASTNode* node);
        InterpretedValue HandleIfStmt(const FrontEnd::ASTNode* node);
        InterpretedValue HandlePrintStmt(const FrontEnd::ASTNode* node);
        InterpretedValue HandleReturnStmt(const FrontEnd::ASTNode* node);
        InterpretedValue HandleScanStmt(const FrontEnd::ASTNode* node);
        InterpretedValue HandleWhileStmt(const FrontEnd::ASTNode* node);
    
    private:
        InterpretedValue DispatchNode(const FrontEnd::ASTNode* node);
        const FrontEnd::Symbol* GetSymbol(const FrontEnd::ASTNode* node) const;

    private:
        const FrontEnd::ASTNode* mCurrentNode;
        std::shared_ptr<FrontEnd::SymbolTable> mSymTable;
        CallStack mCallStack;
    };
}

#endif // INTERPRETER_H__TOSTITOS
