#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "../CFG/module.h"
#include "../Common/astvisitor.h"
#include "../Sema/symboltable.h"

#include <map>

namespace TosLang
{
    namespace FrontEnd
    {
        class FunctionDecl;
        class SymbolTable;
    }

    namespace BackEnd
    {
        /*
         * \class InstructionSelector
         * \brief: TODO
         */
        class InstructionSelector : public Common::ASTVisitor<InstructionSelector>
        {
            friend class Common::ASTVisitor<InstructionSelector>;
            
        public:
            InstructionSelector();
            ~InstructionSelector() = default;

        public:
            std::unique_ptr<Module> Run(const std::unique_ptr<FrontEnd::ASTNode>& root, 
                                        const std::shared_ptr<FrontEnd::SymbolTable>& symTab);

        protected:  // Declarations
            void HandleFunctionDecl();
            void HandleVarDecl();

        protected:  // Expressions
            void HandleBinaryExpr();
            void HandleBooleanExpr();
            void HandleCallExpr();
            void HandleIdentifierExpr();
            void HandleNumberExpr();
            void HandleStringExpr();

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
            unsigned mNextRegister;                                             /*!< Next register number to give out */
            std::map<const FrontEnd::ASTNode*, unsigned> mNodeRegister;         /*!< Mapping indicating in which register an AST node value lives */
            std::unique_ptr<Module> mMod;                                       /*!< Translation unit being built out of the AST */
            FrontEnd::FunctionDecl* mCurrentFunc;                               /*!< Current function (AST version being read) */
            ControlFlowGraph* mCurrentCFG;                                      /*!< Current function (CFG version being written) */
            BasicBlock* mCurrentBlock;                                          /*!< Current basic block being written to */
            std::map<const FrontEnd::ASTNode*, BasicBlock*> mTreeToGraphMap;    /*!< Mapping indicating what basic block an AST node corresponds to */
            std::shared_ptr<FrontEnd::SymbolTable> mSymTable;                   /*!< Symbols associated with the AST being traversed */
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS

