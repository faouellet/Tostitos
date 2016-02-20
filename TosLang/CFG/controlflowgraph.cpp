#include "controlflowgraph.h"

using namespace TosLang::BackEnd;

const BlockPtr& ControlFlowGraph::GetEntryBlock() const
{
    return mBlocks.front();
}

BlockPtr ControlFlowGraph::CreateNewBlock()
{
    BlockPtr newBlock = std::make_shared<BasicBlock>();
    mBlocks.push_back(newBlock);
    return newBlock;
}

BlockPtr ControlFlowGraph::CreateNewBlock(std::vector<VirtualInstruction>&& insts)
{
    BlockPtr newBlock = std::make_shared<BasicBlock>();

    for (auto& inst : insts)
        newBlock->InsertInstruction(inst);

    mBlocks.push_back(newBlock);
    return newBlock;
}
