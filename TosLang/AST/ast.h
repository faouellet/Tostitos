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

        protected:
            /*
            * \fn       AddChildNode
            * \brief    Adds a child node to this AST node
            */
            void AddChildNode(std::unique_ptr<ASTNode>&& node) { mChildren.push_back(std::move(node)); }

        public:
            /*
            * \fn       GetChildrenNodes
            * \brief    Gets the children nodes of the AST node
            * \return   Children nodes of the AST node
            */
            const ChildrenNodes& GetChildrenNodes() const { return mChildren; }

            /*
            * \fn       GetKind
            * \brief    Gets the kind of the AST node
            * \return   Kind of the AST node
            */
            NodeKind GetKind() const { return mKind; }

            /*
            * \fn       GetName
            * \brief    Gets the name of the AST node
            * \return   Name of the AST node
            */
            const std::string& GetName() const { return mName; }

        protected:
            NodeKind mKind;             /*!< Kind of the AST node */
            std::string mName;          /*!< Name of the AST node. In the case of the IdentifierExpr node, it is also the name of the identifier. */
            ChildrenNodes mChildren;    /*!< List of children nodes linked to this AST node */
        };
    }
}

#endif // AST_H__TOSTITOS
