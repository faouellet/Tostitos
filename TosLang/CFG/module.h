#ifndef MODULE__TOSTITOS
#define MODULE__TOSTITOS

#include "controlflowgraph.h"

#include <unordered_map>

namespace TosLang
{
    namespace BackEnd
    {
        using CFGPtr = std::shared_ptr<ControlFlowGraph>;
        using FunctionCFGs = std::unordered_map<std::string, CFGPtr>;

        /*
        * \class Module
        * \brief TODO
        */
        class Module
        {
        public:
            const CFGPtr& GetFunction(const std::string& name) const;

        public:
            void InsertFunction(const std::string& name, const CFGPtr& cfg);
            void InsertGlobalVariable();

        private:
            FunctionCFGs mFuncCFGs;
        };
    }
}

#endif // MODULE__TOSTITOS
