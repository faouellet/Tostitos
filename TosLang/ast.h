#ifndef AST_H__TOSTITOS
#define AST_H__TOSTITOS

#include <memory>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;

        typedef std::vector<std::unique_ptr<ASTNode>> ChildrenNodes;

        /*
        * \class ASTNode
        * \brief Basic clas for every node of the AST
        */
        class ASTNode
        {
        public:
            enum NodeKind
            {
                // Declarations
                PROGRAM_DECL,
                VAR_DECL,

                // Statements

                // Expressions
                BOOLEAN_EXPR,
                NUMBER_EXPR,

                // Misc
                ERROR,
            };

        public:
            ASTNode(NodeKind kind = ERROR) : mKind(kind) { }
            virtual ~ASTNode() { }

            NodeKind GetKind() const { return mKind; }

            void AddChildNode(std::unique_ptr<ASTNode>&& node) { mChildren.push_back(std::move(node)); }
            const ChildrenNodes& GetChildrenNodes() const { return mChildren; }

        protected:
            NodeKind mKind;
            ChildrenNodes mChildren;
        };
    }
}

#endif // AST_H__TOSTITOS
