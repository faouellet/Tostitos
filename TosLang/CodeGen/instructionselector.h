#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "controlflowgraph.h"
#include "../Common/astvisitor.h"

namespace TosLang
{
    namespace BackEnd
    {
        /*
         * \class InstructionSelector
         * \brief: TODO
         */
        class InstructionSelector : Common::ASTVisitor<InstructionSelector>
        {
            friend class Common::ASTVisitor<InstructionSelector>;

        public:
            using Program = std::vector<ControlFlowGraph>;

        public:
            InstructionSelector() : mNextRegister{ 0 } { };
            ~InstructionSelector() = default;

        public:
            Program Execute(const std::unique_ptr<FrontEnd::ASTNode>& root);

        protected:  // Declarations
            void HandleFunctionDecl();
            void HandleVarDecl();

        protected:  // Expressions
            void HandleBinaryExpr();
            void HandleBooleanExpr();
            void HandleCallExpr();
            void HandleIdentifierExpr();
            void HandleNumberExpr();

        protected:  // Statements
            void HandleIfStmt();
            void HandlePrintStmt();
            void HandleReturnStmt();
            void HandleScanStmt();
            void HandleWhileStmt();

        private:
            unsigned mNextRegister;
            Program mProgram;
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS

