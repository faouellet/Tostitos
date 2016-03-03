#ifndef CFG_VISITOR_H__TOSTITOS
#define CFG_VISITOR_H__TOSTITOS

#include "../CFG/controlflowgraph.h"

namespace TosLang
{
    namespace Common
    {
        /*
        * \class CFGVisitor
        * \brief TODO
        */
        template <typename NodeAction>
        class CFGVisitor
        {
        public:
            CFGVisitor() = default;
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

                mAction.Execute(block);
            }

            /*
            * \fn RecursiveDFSVisitPreOrder
            * \brief TODO
            */
            void RecursiveDFSVisitPreOrder(const BlockPtr block)
            {
                mBlocksVisited.push_back(block);

                mAction.Execute(block);

                for (auto& adjBlock : block)
                {
                    if (std::find(mBlocksVisited.begin(), mBlocksVisited.end(), block) == mBlocksVisited.end())
                    {
                        RecursiveDFSVisitPreOrder(adjBlock);
                    }
                }
            }

        private:
            std::vector<BlockPtr> mBlocksVisited;
            NodeAction mAction;
        };
    }
}

#endif //CFG_VISITOR_H__TOSTITOS
