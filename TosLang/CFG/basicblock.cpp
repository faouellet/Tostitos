#include "basicblock.h"

using namespace TosLang::BackEnd;

void BasicBlock::InsertBranch(const BasicBlock* block)
{
    mAdjacentBlocks.push_back(std::make_shared<BasicBlock>(*block));
}

void BasicBlock::InsertBranch(const BlockPtr& block)
{
    mAdjacentBlocks.push_back(block);
}

void BasicBlock::InsertInstruction(VirtualInstruction&& inst)
{
    mInstructions.push_back(inst);
}

void BasicBlock::InsertInstruction(const VirtualInstruction& inst)
{
    mInstructions.push_back(inst);
}

bool BasicBlock::IsProperlyTerminated() const
{
    if (mInstructions.empty())
        return false;

    const VirtualInstruction::Opcode opcode = mInstructions.back().GetOpcode();
    return (opcode == VirtualInstruction::Opcode::JUMP) || (opcode == VirtualInstruction::Opcode::RET);
}
