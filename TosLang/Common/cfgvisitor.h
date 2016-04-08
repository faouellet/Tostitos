#ifndef CFG_VISITOR_H__TOSTITOS
#define CFG_VISITOR_H__TOSTITOS

#include "../CFG/controlflowgraph.h"

#include <algorithm>

namespace TosLang
{
    namespace Common
    {
        /*
        * \class CFGVisitor
        * \brief Utility class that encapsulates the notion of visiting a control flow graph
        */
        template <typename NodeAction>
        class CFGVisitor
        {
        public:
            /*
            * \fn           CFGVisitor
            * \brief        Ctor
            * \param args   Arguments to be given to the action to be applied to each node in the CFG
            */
            template <typename... Args>
            CFGVisitor(Args&&... args) : mAction(std::forward<Args>(args)...) { }

            ~CFGVisitor() = default;

        public:
            /*
            * \fn                   Visit
            * \brief                Visit each node of the CFG (once) and applies an action each times it visit a node
            * \param cfg            Graph to be visited
            * \param postOrderVisit Perform a post-order or a pre-order visit of the CFG?
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
            * \fn           RecursiveDFSVisitPostOrder
            * \brief        Recursively perform a DFS visit of a sub-graph in a post-order fashion
            * \param block  Block at the beginning of the sub-graph we want to visit
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
            * \fn           RecursiveDFSVisitPreOrder
            * \brief        Recursively perform a DFS visit of a sub-graph in a pre-order fashion
            * \param block  Block at the beginning of the sub-graph we want to visit
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
            std::vector<BackEnd::BlockPtr> mBlocksVisited;  /*!< Blocks having been visited so far */
            NodeAction mAction;                             /*!< Action to be performed at each node */
        };
    }
}

#endif //CFG_VISITOR_H__TOSTITOS
