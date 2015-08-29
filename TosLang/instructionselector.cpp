#include "instructionselector.h"

#include <cassert>

using namespace TosLang::FrontEnd;
using namespace TosLang::BackEnd;

std::vector<Instruction> InstructionSelector::Execute(const std::unique_ptr<ASTNode>& root)
{
    this->VisitPostOrder(root);
    return mProgram;
}

// Declarations

void InstructionSelector::HandleProgramDecl()
{

}

void InstructionSelector::HandleVarDecl()
{

}

// Expressions

void InstructionSelector::HandleBooleanExpr()
{
    std::vector<Rule> rules{ mRuleTable.MatchPattern(std::make_pair(NO_OP, std::vector<OperandType>{ IMMEDIATE })) };
    assert(rules.size() == 1);
    mNodeLabels[this->mCurrentNode] = rules[0];
}

void InstructionSelector::HandleNumberExpr()
{
    std::vector<Rule> rules{ mRuleTable.MatchPattern(std::make_pair(NO_OP, std::vector<OperandType>{ IMMEDIATE })) };
    assert(rules.size() == 1);
    mNodeLabels[this->mCurrentNode] = rules[0];
}
