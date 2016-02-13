#include "controlflowgraph.h"

//////////////////// Basic block implementation ////////////////////

void BasicBlock::InsertBranch(const BlockPtr& block)
{
	mAdjacentBlocks.push_back(block);
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
