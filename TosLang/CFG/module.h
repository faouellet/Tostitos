#ifndef MODULE__TOSTITOS
#define MODULE__TOSTITOS

#include "controlflowgraph.h"

namespace TosLang
{
    namespace BackEnd
    {
        using CFGPtr = std::shared_ptr<ControlFlowGraph>;
        using FunctionList = std::vector<CFGPtr>;

        /*
        * \class Module
        * \brief TODO
        */
        class Module
        {
        public:
            void InsertFunction(const CFGPtr& cfg);
            void InsertGlobalVariable();

        private:
            FunctionList mFunc;
        };
    }
}

#endif // MODULE__TOSTITOS
