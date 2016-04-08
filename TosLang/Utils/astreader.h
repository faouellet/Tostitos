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

    class SourceLocation;

    namespace Utils
    {
        /*
        * \class ASTReader
        * \brief Utility class to create an AST from the contents of an .ast file
        */
        class ASTReader
        {
        public:
            /*
            * \fn       ASTReader
            * \brief    Ctor
            */
            ASTReader();

            /*
            * \fn               Run
            * \brief            Creates an AST from the contents of an .ast file
            * \param filename   Name of an .ast file
            * \return           Root node of the generated AST
            */
            std::unique_ptr<FrontEnd::ASTNode> Run(const std::string& filename);

        private:
            /*
            * \fn       ReadSourceLocation
            * \brief    Reads the source location information on the current line
            * \return   Source location of an AST node
            */
            const SourceLocation ReadSourceLocation();

        private:
            // Declarations
            std::unique_ptr<FrontEnd::FunctionDecl> ReadFuncDecl();
            std::unique_ptr<FrontEnd::VarDecl> ReadVarDecl(const bool isFuncParam = false);

            // Expressions
            std::unique_ptr<FrontEnd::Expr> ReadExpr();

            // Statements
            std::unique_ptr<FrontEnd::CompoundStmt> ReadCompoundStmt();
            std::unique_ptr<FrontEnd::Stmt> ReadStmt();

        private:
            std::ifstream mStream;                                                          /*!< Entry stream */
            std::string mCurrentLine;                                                       /*!< Current line being parsed */
            std::smatch mCurrentMatch;                                                      /*!< Current match result */
            std::unordered_map<FrontEnd::ASTNode::NodeKind, std::regex> mNodeKindRegexes;   /*!< Mapping between node kind and regexes */
            std::regex mSrcLocRegex;                                                        /*!< Regex used to get source location */
        };
    }
}

#endif // AST_READER_H__TOSTITOS
