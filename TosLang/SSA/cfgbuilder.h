#ifndef CFG_BUILDER_H__TOSTITOS
#define CFG_BUILDER_H__TOSTITOS

#include "../CFG/module.h"
#include "ssafunction.h"

#include <deque>
#include <memory>
#include <set>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class CompoundStmt;
        class Expr;
        class FunctionDecl;
        class Symbol;
        class SymbolTable;
        class VarDecl;
    }

    namespace BackEnd
    {
        using SSAInstList = std::vector<SSAInstruction>;
        using SSABlock    = BasicBlock<SSAInstruction>;
        using SSABlockPtr = BlockPtr<SSAInstruction>;
        using FuncPtr     = std::shared_ptr<SSAFunction>;
        using SSAModule   = Module<SSAInstruction>;

        /*
         * \class CFGBuilder
         * \brief Builds a graphical representation (CFG) of the program described by given AST. 
         *        The CFG produced is in SSA form to facilitate further optimizations and register allocation.
         *        The algorithm used is largely inspired by: https://pp.info.uni-karlsruhe.de/uploads/publikationen/braun13cc.pdf
         */
        class CFGBuilder
        {
        public:
            CFGBuilder()
                : mNextID{ 0 }, mSymTable{ nullptr },
                  mCurrentVarDef {}, mIncompletePHIs{}, mMod{ nullptr },
                  mCurrentFunction{ nullptr }, mCurrentBlock{ nullptr } { }

        public:
            std::unique_ptr<Module<SSAInstruction>> Run(const std::unique_ptr<FrontEnd::ASTNode>& root, 
                                                        const std::shared_ptr<FrontEnd::SymbolTable>& symTable);

        protected:  // Declarations
            void HandleFunctionDecl(const FrontEnd::ASTNode* decl);
            void HandleProgramDecl(const std::unique_ptr<FrontEnd::ASTNode>& root);
            void HandleVarDecl(const FrontEnd::ASTNode* decl);

        protected:  // Expressions
            const SSAInstruction* HandleExpr(const FrontEnd::Expr* expr);
            const SSAInstruction* HandleBinaryExpr(const FrontEnd::ASTNode* expr);
            void HandleCallExpr(const FrontEnd::ASTNode* expr);

        protected:  // Statements
            /*
            * \fn           HandleCompoundStmt
            * \brief        Creates basic blocks form a compound statement AST node. When this function is done, 
            *               mCurrentBlock points at the last block generated.
            * \param cStmt  Compound statement AST node
            * \return       Pointer to the first block of the statement
            */
            SSABlock* HandleCompoundStmt(const FrontEnd::CompoundStmt* cStmt);
            
            void HandleIfStmt(const FrontEnd::ASTNode* stmt);
            void HandlePrintStmt(const FrontEnd::ASTNode* stmt);
            void HandleReturnStmt(const FrontEnd::ASTNode* stmt);
            void HandleScanStmt(const FrontEnd::ASTNode* stmt);
            void HandleWhileStmt(const FrontEnd::ASTNode* stmt);

        private:
            /*
            * TODO
            */
            const SSAInstruction* AddInstruction(const SSAInstruction& inst);

            /*
            * \fn           CreateNewCurrentBlock
            * \brief        Creates a new basic block. It then adds it to the current CFG by linking it with the previous current block. 
            *               This will change mCurrentBlock so that it will now points to the newly created block.
            * \param insts  Instructions to be inserted in the new current block
            */
            void CreateNewCurrentBlock(SSAInstList&& insts = SSAInstList{});

            /*
            * TODO
            */
            void WriteVariable(const FrontEnd::Symbol* varNode, const SSABlock* block, const SSAValue& value);

            /*
            * TODO
            */
            SSAValue ReadVariable(const FrontEnd::Symbol* variable, const SSABlock* block);

            /*
            * TODO
            */
            SSAValue ReadVariableRecursive(const FrontEnd::Symbol* variable, const SSABlock* block);

            /*
            * TODO
            */
            SSAValue AddPHIOperand(const FrontEnd::Symbol* variable, SSAInstruction* phi);

            /*
            * TODO
            */
            SSAValue TryRemoveTrivialPHI(SSAInstruction* phi);
            
        private:
            using CurrentVarDef = std::unordered_map<const FrontEnd::Symbol*, std::unordered_map<const SSABlock*, SSAValue>>;
            using PHIMapping = std::unordered_map<const SSABlock*, std::unordered_map<const FrontEnd::Symbol*, const SSAInstruction*>>;

            // TODO: Once experimenting is done, use more carefully chosen data structures
        private:
            size_t mNextID;                                     /*!< Next ID to give a value */
            std::shared_ptr<FrontEnd::SymbolTable> mSymTable;   /*!< Symbol table of the program */

            CurrentVarDef mCurrentVarDef;                       /*!< Mapping indicating the latest value taken by a variable in a given block */
            PHIMapping mIncompletePHIs;                         /*!< Mapping indicating the incomplete phi nodes in a given block */
            
            std::unique_ptr<SSAModule> mMod;                    /*!< Translation unit being built out of the AST */
            SSAFunction* mCurrentFunction;                      /*!< Current function being built */
            SSABlock* mCurrentBlock;                            /*!< Current basic block being written to */

            std::set<const SSABlock*> mSealedBlocks;            /*!< Blocks for which no other predecessors will be added */
        };
    }
}

#endif // CFG_BUILDER_H__TOSTITOS

