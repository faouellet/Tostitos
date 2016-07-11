#ifndef EXECUTOR_H__TOSTITOS
#define EXECUTOR_H__TOSTITOS

// TODO: Comments

#include "callstack.h"

#include <deque>
#include <functional>
#include <memory>
#include <stack>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class CallExpr;
        class Symbol;
        class SymbolTable;
    }
}

namespace Threading
{
    namespace impl
    {
        class InterpretedValue;

        class Executor
        {
        public:
            Executor() = default;
            Executor(const TosLang::FrontEnd::ASTNode* root,
                     const TosLang::FrontEnd::SymbolTable* symTab);
            Executor(const TosLang::FrontEnd::ASTNode* root,
                     const TosLang::FrontEnd::SymbolTable* symTab,
                     CallStack&& stack,
                     std::function<void(InterpretedValue)>&& callback);

        public:
            bool ExecuteOne();

        private:  // Declarations
            void HandleFunction(const TosLang::FrontEnd::ASTNode* node);
            void HandleVarDecl(const TosLang::FrontEnd::ASTNode* node);

        private:  // Expressions
            void HandleArrayExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleBinaryExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleBooleanExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleCallExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleIdentifierExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleIndexedExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleNumberExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleSpawnExpr(const TosLang::FrontEnd::ASTNode* node);
            void HandleStringExpr(const TosLang::FrontEnd::ASTNode* node);

        private:  // Statements
            void HandleCompoundStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleIfStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandlePrintStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleReturnStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleScanStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleSleepStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleSyncStmt(const TosLang::FrontEnd::ASTNode* node);
            void HandleWhileStmt(const TosLang::FrontEnd::ASTNode* node);

        private:
            void DispatchNode(const TosLang::FrontEnd::ASTNode* node);
            const TosLang::FrontEnd::Symbol* GetSymbol(const TosLang::FrontEnd::ASTNode* node) const;
            StackFrame PrepareNewFrame(const TosLang::FrontEnd::CallExpr* call);

        private:
            std::stack<std::deque<const TosLang::FrontEnd::ASTNode*>> mNextNodesToRun;
            const TosLang::FrontEnd::SymbolTable* mSymTable;
            CallStack mCallStack;
            std::function<void(InterpretedValue)> mCallback;
        };
    }   // namespace impl
}   // namespace Threading

#endif // EXECUTOR_H__TOSTITOS
