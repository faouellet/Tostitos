#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "../Common/astvisitor.h"

#include "../../Tostitos/machine/instruction.h"

#include <stack>
#include <unordered_map>

namespace TosLang
{
    namespace BackEnd
    {
        class InstructionSelector : Common::ASTVisitor<InstructionSelector>
        {
            friend class Common::ASTVisitor<InstructionSelector>;

        public:
            InstructionSelector() : mNextRegister{ 0 } { };
            ~InstructionSelector() = default;

        public:
            std::vector<MachineEngine::ProcessorSpace::Instruction> Execute(const std::unique_ptr<FrontEnd::ASTNode>& root);

        protected:  // Declarations
            void HandleVarDecl();

        protected:  // Expressions
            void HandleBinaryExpr();

            void HandleBooleanExpr();

            void HandleCallExpr();

            void HandleIdentifierExpr();

            void HandleNumberExpr();

        private:
            std::vector<MachineEngine::ProcessorSpace::Instruction> GenerateProgram();
            MachineEngine::ProcessorSpace::Instruction GenerateInstruction(const FrontEnd::ASTNode* node, 
                                                                           MachineEngine::ProcessorSpace::Instruction::InstructionType iType);

        private:
            unsigned mNextRegister;
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS