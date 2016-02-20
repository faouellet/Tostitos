#ifndef BASIC_BLOCK__TOSTITOS
#define BASIC_BLOCK__TOSTITOS

#include "virtualinstruction.h"

#include <memory>
#include <vector>

namespace TosLang
{
    namespace BackEnd
    {
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
            using inst_iterator = std::vector<VirtualInstruction>::iterator;
            using inst_const_iterator = std::vector<VirtualInstruction>::const_iterator;

            using bb_iterator = BlockList::iterator;
            using bb_const_iterator = BlockList::const_iterator;

        public:
            inst_iterator inst_begin() { return mInstructions.begin(); }
            inst_iterator inst_end() { return mInstructions.begin(); }
            inst_const_iterator inst_begin() const { return mInstructions.begin(); }
            inst_const_iterator inst_end() const { return mInstructions.begin(); }

            bb_iterator bb_begin() { return mAdjacentBlocks.begin(); }
            bb_iterator bb_end() { return mAdjacentBlocks.begin(); }
            bb_const_iterator bb_begin() const { return mAdjacentBlocks.begin(); }
            bb_const_iterator bb_end() const { return mAdjacentBlocks.begin(); }
            
        public:
            void InsertBranch(const BasicBlock* block);
            void InsertBranch(const BlockPtr& block);
            void InsertInstruction(VirtualInstruction&& inst);
            void InsertInstruction(const VirtualInstruction& inst);

        private:
            std::vector<VirtualInstruction> mInstructions;
            BlockList mAdjacentBlocks;
        };
    }
}

#endif // BASIC_BLOCK__TOSTITOS
