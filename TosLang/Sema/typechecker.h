#ifndef TYPE_CHECKER_H__TOSTITOS
#define TYPE_CHECKER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "../Common/type.h"

#include <memory>
#include <stack>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        class BinaryOpExpr;
        class Expr;
        class FunctionDecl;
        class Symbol;
        class SymbolTable;

        /*
        * \class TypeChecker
        * \brief AST pass that verifies if no type error occurs when a variable or a function is used
        */
        class TypeChecker : public Common::ASTVisitor<TypeChecker>
        {
            friend class Common::ASTVisitor<TypeChecker>;

        public:
            TypeChecker();

        public:
            /*
            * \fn           Run
            * \brief        Recursively walk the tree rooted at root to check for type errors
            * \param root   Root of the tree to type check
            * \param symTab Symbol table associated with the given AST
            * \return       Number of errors encountered during type checking
            */
            size_t Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab);
            
        private:
            /*
            * \fn               CheckExprEvaluateToType
            * \brief            Checks if the expression evaluates to a value of a given type
            * \param expr       The expression to be checked
            * \param type       The type to be matched
            * \return           True if the type of the expression matches the type given, else false
            */
            bool CheckExprEvaluateToType(const Expr* expr, Common::Type type);

        protected:  // Declarations
            void HandleVarDecl();

        protected:  // Expressions
            void HandleBooleanExpr();
            void HandleBinaryExpr();
            void HandleCallExpr();
            void HandleIdentifierExpr();
            void HandleNumberExpr();
            void HandleStringExpr();

        protected:  // Statements
            void HandleIfStmt();
            void HandlePrintStmt();
            void HandleReturnStmt();
            void HandleWhileStmt();

        private:
            using NodeTypeMap = std::unordered_map<const ASTNode*, Common::Type>;
            using OverloadMap = std::unordered_map<const ASTNode*, std::vector<const Symbol*>>;
            
        private:
            size_t mErrorCount;                         /*!< Number of errors found by the type checker */
            size_t mCurrentScopeID;                     /*!< Current scope identifier */
            FunctionDecl* mCurrentFunc;                 /*!< Current traversed function */
            std::deque<size_t> mCurrentScopesTraversed; /*!< Path from the current scope to the global scopes */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be used by the type checker */
            NodeTypeMap mNodeTypes;                     /*!< Type of the value produced by an AST node */
            OverloadMap mOverloadMap;                   /*!< Mapping between a function call and its overload set */
        };
    }
}

#endif // TYPE_CHECKER_H__TOSTITOS
