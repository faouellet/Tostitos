#include "controlflowgraph.h"

using namespace TosLang::BackEnd;

const BlockPtr& ControlFlowGraph::GetEntryBlock() const
{
    return mBlocks.front();
}

void ControlFlowGraph::InsertNode(const BlockPtr& node)
{
	mBlocks.push_back(node);
}
