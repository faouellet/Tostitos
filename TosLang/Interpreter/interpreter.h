#ifndef INTERPRETER_H__TOSTITOS
#define INTERPRETER_H__TOSTITOS

#include "callstack.h"

#include <memory>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class SymbolTable;
    }

    class Interpreter
    {
    public:
        Interpreter() { }

    public:
        void Run(const std::unique_ptr<FrontEnd::ASTNode>& root, const std::shared_ptr<FrontEnd::SymbolTable>& symTab);
    
    protected:  // Declarations
        void HandleFunction(const FrontEnd::ASTNode* node);
        void HandleVarDecl(const FrontEnd::ASTNode* node);
    
    protected:  // Expressions
        void HandleBinaryExpr(const FrontEnd::ASTNode* node);
        void HandleBooleanExpr(const FrontEnd::ASTNode* node);
        void HandleCallExpr(const FrontEnd::ASTNode* node);
        void HandleIdentifierExpr(const FrontEnd::ASTNode* node);
        void HandleNumberExpr(const FrontEnd::ASTNode* node);
        void HandleStringExpr(const FrontEnd::ASTNode* node);
    
    protected:  // Statements
        void HandleIfStmt(const FrontEnd::ASTNode* node);
        void HandlePrintStmt(const FrontEnd::ASTNode* node);
        void HandleReturnStmt(const FrontEnd::ASTNode* node);
        void HandleScanStmt(const FrontEnd::ASTNode* node);
        void HandleWhileStmt(const FrontEnd::ASTNode* node);
    
    private:
        void DispatchNode(const FrontEnd::ASTNode* node);

    private:
        const FrontEnd::ASTNode* mCurrentNode;
        std::shared_ptr<FrontEnd::SymbolTable> mSymTable;
        CallStack mCallStack;
        StackFrame mGlobalFrame;
    };
}

#endif // INTERPRETER_H__TOSTITOS
