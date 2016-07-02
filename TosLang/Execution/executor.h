#ifndef EXECUTOR_H__TOSTITOS
#define EXECUTOR_H__TOSTITOS

// TODO: Comments

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
    class Executor
    {
    public:

    public:
        void Run(const TosLang::FrontEnd::ASTNode* root,
                 const TosLang::FrontEnd::SymbolTable* symTab,
                 CallStack&& stack);

    private:  // Declarations
        InterpretedValue HandleFunction(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleVarDecl(const TosLang::FrontEnd::ASTNode* node);

    private:  // Expressions
        InterpretedValue HandleArrayExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleBinaryExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleBooleanExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleCallExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleIdentifierExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleIndexedExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleNumberExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleSpawnExpr(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleStringExpr(const TosLang::FrontEnd::ASTNode* node);

    private:  // Statements
        InterpretedValue HandleCompoundStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleIfStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandlePrintStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleReturnStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleScanStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleSleepStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleSyncStmt(const TosLang::FrontEnd::ASTNode* node);
        InterpretedValue HandleWhileStmt(const TosLang::FrontEnd::ASTNode* node);

    private:
        InterpretedValue DispatchNode(const TosLang::FrontEnd::ASTNode* node);
        const TosLang::FrontEnd::Symbol* GetSymbol(const TosLang::FrontEnd::ASTNode* node) const;

    private:
        bool mDoneWithCurrentFunc;
        const TosLang::FrontEnd::ASTNode* mCurrentNode;
        const TosLang::FrontEnd::SymbolTable* mSymTable;
        CallStack mCallStack;
    };
}

#endif // EXECUTOR_H__TOSTITOS
