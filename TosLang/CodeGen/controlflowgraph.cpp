#include "controlflowgraph.h"

using namespace TosLang::BackEnd;

//////////////////// Basic block implementation ////////////////////

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

//////////////////// Control flow graph implementation ////////////////////

void ControlFlowGraph::InsertNode(const BlockPtr& node)
{
	mNodes.push_back(node);
}
