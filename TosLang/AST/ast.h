#ifndef AST_H__TOSTITOS
#define AST_H__TOSTITOS

#include <memory>
#include <string>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;

        using ChildrenNodes = std::vector<std::unique_ptr<ASTNode>>;

        /*
        * \class ASTNode
        * \brief Basic clas for every node of the AST
        */
        class ASTNode
        {
        public:
            enum class NodeKind : unsigned int
            {
                // Declarations
                PROGRAM_DECL,
                VAR_DECL,

                // Statements

                // Expressions
				BINARY_EXPR,
                BOOLEAN_EXPR,
                IDENTIFIER_EXPR,
                NUMBER_EXPR,

                // Misc
                ERROR,
            };

        public:
			explicit ASTNode(NodeKind kind = NodeKind::ERROR) : mKind(kind), mName("") { }
            virtual ~ASTNode() { }

            NodeKind GetKind() const { return mKind; }
            const std::string& GetName() const { return mName; }

            void AddChildNode(std::unique_ptr<ASTNode>&& node) { mChildren.push_back(std::move(node)); }
            const ChildrenNodes& GetChildrenNodes() const { return mChildren; }

        protected:
            NodeKind mKind;
            std::string mName;
            ChildrenNodes mChildren;
        };
    }
}

#endif // AST_H__TOSTITOS
