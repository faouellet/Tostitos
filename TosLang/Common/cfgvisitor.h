#ifndef CFG_VISITOR_H__TOSTITOS
#define CFG_VISITOR_H__TOSTITOS

#include "../CFG/controlflowgraph.h"

namespace TosLang
{
    namespace Backend
    {
        /*
        * \class CFGVisitor
        * \brief TODO
        */
        template <typename NodeAction>
        class CFGVisitor
        {
        public:
            CFGVisitor() : mCurrentBlock(nullptr) { }
            ~CFGVisitor() = default;

        protected:
            /*
            * \fn DFSVisit
            * \brief TODO
            */
            void Visit(const std::shared_ptr<ControlFlowGraph> cfg, bool postOrderVisit)
            {
                BlockPtr entry = cfg.GetEntryBlock();
                if (postOrderVisit)
                    RecursiveDFSVisitPostOrder(entry);
                else
                    RecursiveDFSVisitPreOrder(entry);
            }

        private:
            /*
            * \fn RecursiveDFSVisitPostOrder
            * \brief TODO
            */
            void RecursiveDFSVisitPostOrder(const BlockPtr block)
            {
                mBlocksVisited.push_back(block);

                for (auto& adjBlock : block)
                {
                    if (std::find(mBlocksVisited.begin(), mBlocksVisited.end(), block) == mBlocksVisited.end())
                    {
                        RecursiveDFSVisitPostOrder(adjbBlock);
                    }
                }

                NodeAction::Execute();
            }

            /*
            * \fn RecursiveDFSVisitPreOrder
            * \brief TODO
            */
            void RecursiveDFSVisitPreOrder(const BlockPtr block)
            {
                mBlocksVisited.push_back(block);

                NodeAction::Execute();

                for (auto& adjBlock : block)
                {
                    if (std::find(mBlocksVisited.begin(), mBlocksVisited.end(), block) == mBlocksVisited.end())
                    {
                        RecursiveDFSVisitPreOrder(adjBlock);
                    }
                }
            }

        private:
            BasicBlock* mCurrentBlock;
            std::vector<BlockPtr> mBlocksVisited;
        };
    }
}

#endif //CFG_VISITOR_H__TOSTITOS
