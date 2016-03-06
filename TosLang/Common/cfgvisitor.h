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

        public:
            /*
            * \fn Visit
            * \brief TODO
            */
            void Visit(const std::shared_ptr<BackEnd::ControlFlowGraph> cfg, bool postOrderVisit)
            {
                BackEnd::BlockPtr entry = cfg->GetEntryBlock();
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
            void RecursiveDFSVisitPostOrder(const BackEnd::BlockPtr block)
            {
                mBlocksVisited.push_back(block);

                for (auto bbIt = block->bb_begin(), bbEnd = block->bb_end(); bbIt != bbEnd; ++bbIt)
                {
                    if (std::find(mBlocksVisited.begin(), mBlocksVisited.end(), *bbIt) == mBlocksVisited.end())
                    {
                        RecursiveDFSVisitPostOrder(*bbIt);
                    }
                }

                mAction.Execute(block);
            }

            /*
            * \fn RecursiveDFSVisitPreOrder
            * \brief TODO
            */
            void RecursiveDFSVisitPreOrder(const BackEnd::BlockPtr block)
            {
                mBlocksVisited.push_back(block);

                mAction.Execute(block);

                for (auto bbIt = block->bb_begin(), bbEnd = block->bb_end(); bbIt != bbEnd; ++bbIt)
                {
                    if (std::find(mBlocksVisited.begin(), mBlocksVisited.end(), *bbIt) == mBlocksVisited.end())
                    {
                        RecursiveDFSVisitPreOrder(*bbIt);
                    }
                }
            }

        private:
            std::vector<BackEnd::BlockPtr> mBlocksVisited;
            NodeAction mAction;
        };
    }
}

#endif //CFG_VISITOR_H__TOSTITOS
