#ifndef OVERLOAD_SOLVER_H__TOSTITOS
#define OVERLOAD_SOLVER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "utils.h"

namespace TosLang
{
    namespace FrontEnd
    {
        class SymbolTable;

        /*
        * TODO
        */
        class OverloadSolver : public Common::ASTVisitor<OverloadSolver>
        {
            friend class Common::ASTVisitor<OverloadSolver>;

        public:
            OverloadSolver() = default;

        public:
            size_t Run(const std::unique_ptr<ASTNode>& root, 
                       const std::shared_ptr<SymbolTable>& symTab, 
                       const std::shared_ptr<NodeTypeMap>& nodeTypes);

        protected:
            void HandleCallExpr();

        private:
            size_t mErrorCount;
            std::shared_ptr<SymbolTable> mSymTable;
            std::shared_ptr<NodeTypeMap> mNodeTypes;
        };
    }
}

#endif // OVERLOAD_SOLVER_H__TOSTITOS
