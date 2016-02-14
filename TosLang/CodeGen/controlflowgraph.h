#ifndef CONTROL_FLOW_GRAPH__TOSTITOS
#define CONTROL_FLOW_GRAPH__TOSTITOS

#include "../../Tostitos/machine/instruction.h"

#include <memory>
#include <vector>

using MachineEngine::ProcessorSpace::Instruction;

namespace TosLang
{
    namespace BackEnd
    {
        /*
        * \class VirtualInstruction
        * \brief TODO
        */
        class VirtualInstruction
        {
        public:
            VirtualInstruction(Instruction::InstructionOpCode opcode, unsigned op1 = 0, unsigned op2 = 0, unsigned op3 = 0)
                : mOpCode{ opcode }, mOperand1{ op1 }, mOperand2{ op2 }, mOperand3{ op3 } { }

        private:
            Instruction::InstructionOpCode mOpCode;
            unsigned mOperand1;
            unsigned mOperand2;
            unsigned mOperand3;
        };

        class BasicBlock;

        using BlockPtr = std::shared_ptr<BasicBlock>;
        using BlockList = std::vector<BlockPtr>;

        /*
        * \class BasicBlock
        * \brief TODO
        */
        class BasicBlock
        {
        public:
            using iterator = std::vector<VirtualInstruction>::iterator;
            using const_iterator = std::vector<VirtualInstruction>::const_iterator;

        public:
            BasicBlock() = default;
            ~BasicBlock() = default;

        public:
            iterator begin() { return mInstructions.begin(); }
            iterator end() { return mInstructions.begin(); }
            const_iterator begin() const { return mInstructions.begin(); }
            const_iterator end() const { return mInstructions.begin(); }

        public:
            void InsertBranch(const BasicBlock* block);
            void InsertBranch(const BlockPtr& block);
            void InsertInstruction(VirtualInstruction&& inst);
            void InsertInstruction(const VirtualInstruction& inst);

        private:
            std::vector<VirtualInstruction> mInstructions;
            BlockList mAdjacentBlocks;
        };

        /*
        * \class ControlFlowGraph
        * \brief TODO
        */
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
    }
}

#endif // CONTROL_FLOW_GRAPH__TOSTITOS
