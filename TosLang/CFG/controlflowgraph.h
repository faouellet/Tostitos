#ifndef CONTROL_FLOW_GRAPH__TOSTITOS
#define CONTROL_FLOW_GRAPH__TOSTITOS

#include "basicblock.h"

namespace TosLang
{
    namespace BackEnd
    {
        /*
        * \class ControlFlowGraph
        * \brief Representation of a function in a way that encodes information about 
        *        the control flow (branches that can be taken during execution) within it.
        */
        class ControlFlowGraph
        {
        public:
            /*
            * \fn GetEntryBlock
            * \brief Gives access to the entry block in the graph
            * \return Graph entry block
            */
            const BlockPtr& GetEntryBlock() const;

            /*
            * \fn       CreateNewBlock
            * \brief    Creates a new block in the CFG. It is the caller's responsibility 
            *           to correctly link the new block with the others in the CFG
            * \return   Newly created block
            */
            BlockPtr CreateNewBlock();

            /*
            * \fn           CreateNewBlock
            * \brief        Creates a new block in the CFG. It is the caller's responsibility
            *               to correctly link the new block with the others in the CFG
            * \param insts  Instructions to be inserted in the block to create
            * \return       Newly created block
            */
            BlockPtr CreateNewBlock(std::vector<VirtualInstruction>&& insts);

        private:
            BlockList mBlocks;  /*!< Blocks contained in the CFG */
        };
    }
}

#endif // CONTROL_FLOW_GRAPH__TOSTITOS
