#ifndef AST_READER_H__TOSTITOS
#define AST_READER_H__TOSTITOS

#include "../AST/ast.h"

#include <fstream>
#include <regex>
#include <unordered_map>

namespace std
{
    template<>
    struct hash<TosLang::FrontEnd::ASTNode::NodeKind>
    {
        size_t operator()(const TosLang::FrontEnd::ASTNode::NodeKind& kind) const
        {
            return std::hash<std::underlying_type<TosLang::FrontEnd::ASTNode::NodeKind>::type>()(static_cast<std::underlying_type<TosLang::FrontEnd::ASTNode::NodeKind>::type>(kind));
        }
    };
}

namespace TosLang
{
    namespace FrontEnd
    {
        class CompoundStmt;
        class Expr;
        class FunctionDecl;
        class Stmt;
        class VarDecl;
    }

    namespace Utils
    {
        /*
        * \class ASTReader
        * \brief TODO
        */
        class ASTReader
        {
        public:
            ASTReader();
            std::unique_ptr<FrontEnd::ASTNode> Run(const std::string& filename);

        private:
            // Declarations
            std::unique_ptr<FrontEnd::FunctionDecl> ReadFuncDecl();
            std::unique_ptr<FrontEnd::VarDecl> ReadVarDecl();

            // Expressions
            std::unique_ptr<FrontEnd::Expr> ReadExpr();

            // Statements
            std::unique_ptr<FrontEnd::CompoundStmt> ReadCompoundStmt();
            std::unique_ptr<FrontEnd::Stmt> ReadStmt();

        private:
            std::ifstream mStream;
            std::string mCurrentLine;
            std::smatch mCurrentMatch;
            std::unordered_map<FrontEnd::ASTNode::NodeKind, std::regex> mNodeKindRegexes;
        };
    }
}

#endif // AST_READER_H__TOSTITOS
