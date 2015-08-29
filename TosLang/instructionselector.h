#ifndef INSTRUCTION_SELECTOR_H__TOSTITOS
#define INSTRUCTION_SELECTOR_H__TOSTITOS

#include "astvisitor.h"

#include "ruletable.h"

#include <map>

namespace TosLang
{
    namespace BackEnd
    {
        class InstructionSelector : Utils::ASTVisitor<InstructionSelector>
        {
            friend class Utils::ASTVisitor<InstructionSelector>;

        public:
            InstructionSelector() = default;
            ~InstructionSelector() = default;

        public:
            std::vector<Instruction> Execute(const std::unique_ptr<FrontEnd::ASTNode>& root);

        protected:  // Declarations
            void HandleProgramDecl();
            void HandleVarDecl();

        protected:  // Expressions
            void HandleBooleanExpr();
            void HandleNumberExpr();

        private:
            RuleTable mRuleTable;

            std::map<const FrontEnd::ASTNode*, Rule> mNodeLabels;
            std::vector<Instruction> mProgram;
        };
    }
}

#endif // INSTRUCTION_SELECTOR_H__TOSTITOS