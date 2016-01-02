#ifndef CONTROL_FLOW_GRAPH__TOSTITOS
#define CONTROL_FLOW_GRAPH__TOSTITOS

#include "../../Tostitos/machine/instruction.h"

#include <memory>
#include <vector>

using MachineEngine::ProcessorSpace::Instruction;

class BasicBlock;

using BlockPtr = std::shared_ptr<BasicBlock> ;
using BlockList = std::vector<BlockPtr>;

class BasicBlock
{
public:
	using iterator = std::vector<Instruction>::iterator;
	using const_iterator = std::vector<Instruction>::const_iterator;

public:
	BasicBlock() = default;
	~BasicBlock() = default;

public:
	iterator begin() { return mInstructions.begin(); }
	iterator end() { return mInstructions.begin(); }
	const_iterator begin() const { return mInstructions.begin(); }
	const_iterator end() const { return mInstructions.begin(); }

public:
	void InsertBranch(const BlockPtr& block);
	void InsertInstruction(const Instruction& inst);

private:
	std::vector<Instruction> mInstructions;
	BlockList mAdjacentBlocks;
};

class ControlFlowGraph
{
public:
	ControlFlowGraph() = default;
	~ControlFlowGraph() = default;

public:
	void InsertNode(const BlockPtr& node);

private:
	BlockList mNodes;
};

#endif // CONTROL_FLOW_GRAPH__TOSTITOS
