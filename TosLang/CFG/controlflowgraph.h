#ifndef CONTROL_FLOW_GRAPH__TOSTITOS
#define CONTROL_FLOW_GRAPH__TOSTITOS

#include "basicblock.h"

namespace TosLang
{
    namespace BackEnd
    {
        /*
        * \class ControlFlowGraph
        * \brief TODO
        */
        class ControlFlowGraph
        {
        public:
            const BlockPtr& GetEntryBlock() const;
            void InsertNode(const BlockPtr& node);

        private:
            BlockList mBlocks;
        };
    }
}

#endif // CONTROL_FLOW_GRAPH__TOSTITOS
