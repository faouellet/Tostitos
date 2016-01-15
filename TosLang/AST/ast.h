#ifndef AST_H__TOSTITOS
#define AST_H__TOSTITOS

#include "../Utils/sourceloc.h"

#include <cassert>
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
                FUNCTION_DECL,
                PROGRAM_DECL,
                PARAM_VAR_DECL,
                VAR_DECL,

                // Statements

                // Expressions
                BINARY_EXPR,
                BOOLEAN_EXPR,
                CALL_EXPR,
                IDENTIFIER_EXPR,
                NUMBER_EXPR,

                // Misc
                ERROR,

                // Statements
                COMPOUND_STMT,
                IF_STMT,
                PRINT_STMT,
                RETURN_STMT,
                SCAN_STMT,
                WHILE_STMT,
            };

        public:
            explicit ASTNode(NodeKind kind = NodeKind::ERROR) : mKind{ kind }, mName{ "" } { }
            virtual ~ASTNode() = default;

            ASTNode(const ASTNode&) = default;
            ASTNode(ASTNode&&) = default;
            ASTNode& operator=(const ASTNode&) = default;
            ASTNode& operator=(ASTNode&&) = default;

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

            /*
            * \fn       GetSourceLocation
            * \brief    Gets the location in the source code associated with the AST node
            * \return   Source location
            */
            const Utils::SourceLocation& GetSourceLocation() const { return mSrcLoc; }

        protected:
            /*
            * \fn       AddChildNode
            * \brief    Adds a child node to this AST node
            */
            void AddChildNode(std::unique_ptr<ASTNode>&& node) { mChildren.push_back(std::move(node)); }

            /*
            * \fn               GetChildNodeAs
            * \brief            Gets pointer of the specified type to a child node 
            * \param childIdx   Index of the child node
            * \return           Pointer of the given type to the child node
            */
            template <class T> 
            T* GetChildNodeAs(size_t childIdx) const
            {
                assert(childIdx < mChildren.size()); 
                return dynamic_cast<T*>(mChildren[childIdx].get()); 
            }

        protected:
            NodeKind mKind;                 /*!< Kind of the AST node */
            std::string mName;              /*!< Name of the AST node. In the case of the IdentifierExpr node, it is also the name of the identifier. */
            ChildrenNodes mChildren;        /*!< List of children nodes linked to this AST node */
            Utils::SourceLocation mSrcLoc;  /*!< Location in the source code from which the node was created */
        };
    }
}

#endif // AST_H__TOSTITOS
