#ifndef AST_READER_H__TOSTITOS
#define AST_READER_H__TOSTITOS

#include "../AST/ast.h"

#include <fstream>
#include <regex>
#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        class Expr;
        class FunctionDecl;
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

        private:
            std::ifstream mStream;
            std::string mCurrentLine;
            std::smatch mCurrentMatch;
            std::unordered_map<FrontEnd::ASTNode::NodeKind, std::regex> mNodeKindRegexes;
        };
    }
}

#endif // AST_READER_H__TOSTITOS
