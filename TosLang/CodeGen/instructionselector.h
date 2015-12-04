#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "../Common/astvisitor.h"

#include "ruletable.h"

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
            std::vector<Instruction> Execute(const std::unique_ptr<FrontEnd::ASTNode>& root);

        protected:  // Declarations
            void HandleVarDecl();

        protected:  // Expressions

        private:
            std::vector<Instruction> GenerateProgram();
            Instruction GenerateInstruction(const FrontEnd::ASTNode* node, Instruction::InstructionType iType);

        private:
            unsigned mNextRegister;

            RuleTable mRuleTable;

            std::unordered_map<const FrontEnd::ASTNode*, unsigned> mNodeLabels;
            std::stack<const FrontEnd::ASTNode*> mNodesVisited;
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS