#ifndef BASIC_BLOCK__TOSTITOS
#define BASIC_BLOCK__TOSTITOS

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace TosLang
{
    namespace BackEnd
    {
        template <class InstT>
        class BasicBlock;

        template <class InstT>
        using BlockPtr = std::shared_ptr<BasicBlock<InstT>>;
        
        template <class InstT>
        using BlockList = std::vector<BlockPtr<InstT>>;

        /*
        * \class BasicBlock
        * \brief A basic block is a list of instructions having the following properties:
        *        - If one instruction in the basic block is executed, all other instructions 
        *          in the basic block are guaranteed to be executed
        *        - Instructions will be executed sequentially
        *        - The last instruction in a basic block is either a jump or a return
        */
        template <class InstT>
        class BasicBlock
        {
        public:
            /*
            * \fn           BasicBlock
            * \brief        Ctor
            * \param name   Name of the basic block. If none is given, the block will be named 'BlockN' 
            *               where N is a monotonically increasing number
            */
            BasicBlock(const std::string& name = "") : mInstructions{}
            {
                // OK since we won't create multiple blocks in parallel
                static size_t blockNumber = 0;

                if (name.empty())
                    mName = "Block" + std::to_string(blockNumber++);
                else
                    mName = name;
            }

        public:
            using inst_iterator = typename std::vector<std::unique_ptr<InstT>>::iterator;
            using inst_const_iterator = typename std::vector<std::shared_ptr<InstT>>::const_iterator;

            using bb_iterator = typename BlockList<InstT>::iterator;
            using bb_const_iterator = typename BlockList<InstT>::const_iterator;

        public:
            inst_iterator inst_begin() { return mInstructions.begin(); }
            inst_iterator inst_end() { return mInstructions.end(); }
            inst_const_iterator inst_begin() const { return mInstructions.begin(); }
            inst_const_iterator inst_end() const { return mInstructions.end(); }

            bb_iterator succ_begin() { return mSuccBlocks.begin(); }
            bb_iterator succ_end() { return mSuccBlocks.end(); }
            bb_const_iterator succ_begin() const { return mSuccBlocks.begin(); }
            bb_const_iterator succ_end() const { return mSuccBlocks.end(); }

            bb_iterator pred_begin() { return mPredBlocks.begin(); }
            bb_iterator pred_end() { return mPredBlocks.end(); }
            bb_const_iterator pred_begin() const { return mPredBlocks.begin(); }
            bb_const_iterator pred_end() const { return mPredBlocks.end(); }
            
        public:
            /*
            * \fn           InsertBranch
            * \brief        Adds a branch to a basic block. Note that this function 
            *               is not responsible for adding machine instructions to the basic block.
            * \param block  Basic block to branch to 
            */
            void InsertBranch(BasicBlock<InstT>* block) { mSuccBlocks.emplace_back(std::shared_ptr<BasicBlock<InstT>>(block)); }

            /*
            * \fn           InsertBranch
            * \brief        Adds a branch to a basic block. Note that this function 
            *               is not responsible for adding machine instructions to the basic block.
            * \param block  Basic block to branch to
            */
            void InsertBranch(const BlockPtr<InstT>& block) { mSuccBlocks.push_back(block); }

            /*
            * \fn           InsertInstruction
            * \brief        Appends a virtual instruction to the basic block
            * \param inst   Instruction to be added
            */
            void InsertInstruction(InstT&& inst) { mInstructions.emplace_back(std::make_unique<InstT>(inst)); }

            /*
            * \fn           InsertInstruction
            * \brief        Appends a virtual instruction to the basic block
            * \param inst   Instruction to be added
            */
            void InsertInstruction(const InstT& inst) { mInstructions.emplace_back(std::make_unique<InstT>(inst)); }

            /*
            * \fn               ReplaceInstruction
            * \brief            Replaces an instruction in the block
            * \param oldInst    Old instruction
            * \param newInst    Instruction to replace to old one
            */
            void ReplaceInstruction(const InstT& oldInst, const InstT& newInst)
            {
                auto instIt = std::find_if(mInstructions.begin(), mInstructions.end(), 
                                           [&oldInst](const std::unique_ptr<InstT>& inst) 
                                           { 
                                               return *inst == oldInst;
                                           });

                if (instIt != mInstructions.end())
                {
                    const size_t instIdx = std::distance(mInstructions.begin(), instIt);
                    mInstructions[instIdx] = std::make_unique<InstT>(newInst);
                }
            }

        public:
            /*
            * \fn       GetName
            * \brief    Gives access to the name of the basic block
            * \return   Name of the basic block
            */
            const std::string& GetName() const { return mName; }

            /*
            * \fn       GetNbInstructions
            * \brief    Indicates the number of instructions in the basic block
            * \return   The number of instructions in the basic block
            */
            size_t GetNbInstructions() const { return mInstructions.size(); }

            /*
            * \fn       GetPredecessors
            * \brief    Gives access to the block's predecessors
            * \return   The block's predecessors
            */
            const BlockList<InstT>& GetPredecessors() const { return mPredBlocks; }

            /*
            * \fn       GetSuccessors
            * \brief    Gives access to the block's successors
            * \return   The block's successors
            */
            const BlockList<InstT>& GetSuccessors() const { return mSuccBlocks; }

            /*
            * TODO
            */
            InstT* GetTerminator() const { return mInstructions.back().get(); }

        private:
            std::vector<std::unique_ptr<InstT>> mInstructions;  /*!< Instructions making up the basic block */
            BlockList<InstT> mSuccBlocks;                       /*!< List of blocks pointed to by the outgoing edges of the block */
            BlockList<InstT> mPredBlocks;                       /*!< List of blocks that points to the block */
            std::string mName;                                  /*<! Name of the basic block. For printing and debugginf purposes */
        };
    }
}

#endif // BASIC_BLOCK__TOSTITOS
