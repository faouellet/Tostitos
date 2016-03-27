#ifndef LLVM_GENERATOR_H__TOSTITO
#define LLVM_GENERATOR_H__TOSTITOS

#include "../Sema/symboltable.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <map>

namespace llvm
{
    class Function;
    class Value;
}

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
        class LLVMGenerator
        {
        public:
            LLVMGenerator() 
                : mMod{ nullptr }, mBuilder{}, mSymTable{ nullptr } { }

        public:
            std::unique_ptr<Module> Run(const std::unique_ptr<FrontEnd::ASTNode>& root,
                                        const std::shared_ptr<FrontEnd::SymbolTable>& symTab);

        protected:  // Declarations
            llvm::Function* HandleFunctionDecl(const FrontEnd::ASTNode* decl);
            void HandleProgramDecl(const std::unique_ptr<FrontEnd::ASTNode>& root);
            void HandleVarDecl(const FrontEnd::ASTNode* decl);

        protected:  // Expressions
            llvm::Value* HandleExpr(const FrontEnd::Expr* expr);
            llvm::Value* HandleBinaryExpr(const FrontEnd::ASTNode* expr);
            llvm::Value* HandleCallExpr(const FrontEnd::ASTNode* expr);

        protected:  // Statements
            void HandleCompoundStmt(const FrontEnd::CompoundStmt* cStmt);
            void HandleIfStmt(const FrontEnd::ASTNode* stmt);
            void HandlePrintStmt(const FrontEnd::ASTNode* stmt);
            void HandleReturnStmt(const FrontEnd::ASTNode* stmt);
            void HandleScanStmt(const FrontEnd::ASTNode* stmt);
            void HandleWhileStmt(const FrontEnd::ASTNode* stmt);

        private:
            std::unique_ptr<Module> mMod;                                       /*!< Translation unit being built out of the AST */
            IRBuilder<> mBuilder;
            std::shared_ptr<FrontEnd::SymbolTable> mSymTable;                   /*!< Symbols associated with the AST being traversed */
        };
    }
}

#endif // LLVM_GENERATOR_H__TOSTITOS

