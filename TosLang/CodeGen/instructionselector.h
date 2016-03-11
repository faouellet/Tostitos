#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "../CFG/module.h"
#include "../Sema/symboltable.h"

#include <map>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class CompoundStmt;
        class Expr;
        class SymbolTable;
    }

    namespace BackEnd
    {
        /*
         * \class InstructionSelector
         * \brief: TODO
         */
        class InstructionSelector
        {
        public:
            InstructionSelector() 
                : mNextRegister{ 0 }, mNodeRegister{ }, mMod{ nullptr }, 
                  mCurrentCFG{ nullptr }, mCurrentBlock{ nullptr }, mSymTable{ nullptr } { }

        public:
            std::unique_ptr<Module> Run(const std::unique_ptr<FrontEnd::ASTNode>& root, 
                                        const std::shared_ptr<FrontEnd::SymbolTable>& symTab);

        protected:  // Declarations
            void HandleFunctionDecl(const FrontEnd::ASTNode* decl);
            void HandleProgramDecl(const std::unique_ptr<FrontEnd::ASTNode>& root);
            void HandleVarDecl(const FrontEnd::ASTNode* decl);

        protected:  // Expressions
            void HandleExpr(const FrontEnd::Expr* expr);
            void HandleBinaryExpr(const FrontEnd::ASTNode* expr);
            void HandleCallExpr(const FrontEnd::ASTNode* expr);

        protected:  // Statements
            /*
            * \fn           HandleCompoundStmt
            * \brief        Creates basic blocks form a compound statement AST node. When this function is done, 
            *               mCurrentBlock points at the last block generated.
            * \param cStmt  Compound statement AST node
            * \return       Pointer to the first block of the statement
            */
            BlockPtr HandleCompoundStmt(const FrontEnd::CompoundStmt* cStmt);
            
            void HandleIfStmt(const FrontEnd::ASTNode* stmt);
            void HandlePrintStmt(const FrontEnd::ASTNode* stmt);
            void HandleReturnStmt(const FrontEnd::ASTNode* stmt);
            void HandleScanStmt(const FrontEnd::ASTNode* stmt);
            void HandleWhileStmt(const FrontEnd::ASTNode* stmt);

        private:
            /*
            * \fn           CreateNewCurrentBlock
            * \brief        Creates a new basic block. It then adds it to the current CFG by linking it with the previous current block. 
            *               This will change mCurrentBlock so that it will now points to the newly created block.
            * \param insts  Instructions to be inserted in the new current block
            */
            void CreateNewCurrentBlock(std::vector<VirtualInstruction>&& insts = std::vector<VirtualInstruction>{});

        private:
            unsigned mNextRegister;                                             /*!< Next register number to give out */
            std::map<const FrontEnd::ASTNode*, unsigned> mNodeRegister;         /*!< Mapping indicating in which register an AST node value lives */
            std::unique_ptr<Module> mMod;                                       /*!< Translation unit being built out of the AST */
            ControlFlowGraph* mCurrentCFG;                                      /*!< Current function (CFG version being written) */
            BasicBlock* mCurrentBlock;                                          /*!< Current basic block being written to */
            std::shared_ptr<FrontEnd::SymbolTable> mSymTable;                   /*!< Symbols associated with the AST being traversed */
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS

