#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "../CFG/controlflowgraph.h"
#include "../Common/astvisitor.h"
#include "../Sema/symboltable.h"

#include <map>

namespace TosLang
{
    namespace BackEnd
    {
        class FunctionDecl;

        /*
         * \class InstructionSelector
         * \brief: TODO
         */
        class InstructionSelector : public Common::ASTVisitor<InstructionSelector>
        {
            friend class Common::ASTVisitor<InstructionSelector>;

        public:
            using FunctionCFGs = std::map<FunctionDecl*, ControlFlowGraph>;

        public:
            InstructionSelector(const std::shared_ptr<FrontEnd::SymbolTable>& symTab);
            ~InstructionSelector() = default;

        public:
            FunctionCFGs Run(const std::unique_ptr<FrontEnd::ASTNode>& root);

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
            /*
            * \fn           CreateNewCurrentBlock
            * \brief        Creates a new basic block. It then adds it to the current CFG by linking it with the previous current block. 
            *               This will change mCurrentBlock so that it will now points to the newly created block.
            * \param insts  Instructions to be inserted in the new current block
            */
            void CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts);

        private:
            unsigned mNextRegister;
            std::shared_ptr<FrontEnd::SymbolTable> mSymTable;
            std::map<const FrontEnd::ASTNode*, unsigned> mNodeRegister;
            FunctionCFGs mFunctionGraphs;
            FunctionDecl* mCurrentFunc;                 /*!< Currently traversed function */
            ControlFlowGraph mCurrentCFG;
            BasicBlock* mCurrentBlock;
            std::map<const FrontEnd::ASTNode*, BasicBlock*> mTreeToGraphMap;
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS

