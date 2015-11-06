#ifndef PARSER_H__TOSTITOS
#define PARSER_H__TOSTITOS

#include "lexer.h"
#include "symboltable.h"

#include <memory>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class Expr;
        class FunctionDecl;
        class VarDecl;
        
        /*
        * \class Parser
        * \brief Part of the TosLang compiler used to generate an AST
        */
        class Parser
        {
        public:
            explicit Parser(const std::shared_ptr<SymbolTable>& symTab) : mSymbolTable(symTab), mCurrentToken{ Lexer::Token::UNKNOWN } { }
            ~Parser() = default;

            /*
            * \fn               ParseProgram
            * \brief            Generate an AST from
            * \param filename   Name of a TosLang file
            * \return           Root node of the AST
            */
            std::unique_ptr<ASTNode> ParseProgram(const std::string& filename);

        private:
            /*
            * \fn       GoToNextStmt
            * \brief    Go to the next statement in the program. This is used as part of the error recovery mechanism of the parser.
            */
            void GoToNextStmt();

        private:	// Declarations
            /*
            * \fn       ParseProgramDecl
            * \brief    programdecl
            *                   ::= decls
            *                   ::= stmts
            * \return   A node representing a program
            */
            std::unique_ptr<ASTNode> ParseProgramDecl();

            /*
            * \fn       ParseFunctionDecl
            * \brief    functiondecl ::= 'fn' identifierexpr '(' ( vardecl, )* ')' '->' typeexpr
            * \return   A node representing a function declaration or definition
            */
            std::unique_ptr<FunctionDecl> ParseFunctionDecl();

            /*
            * \fn       ParseVarDecl
            * \brief    vardecl ::= 'var' identifierexpr ':' typeexpr ( '=' expr ) ';'
            * \return   A node representing a variable declaration with potentially an initializing expression
            */
            std::unique_ptr<VarDecl> ParseVarDecl();

		private:	// Expressions
			/*
			* \fn       ParseExpr
			* \brief    expr 
			*               ::= binopexpr
            *               ::= booleanexpr
            *               ::= identifierexpr
            *               ::= numberexpr
			* \return   A node representing an expression
			*/
			std::unique_ptr<Expr> ParseExpr();

			/*
			* \fn           ParseBinaryOpExpr
			* \brief        binopexpr ::= expr OP expr
            * \param lhs    The left hand side expression of the binary expression
			* \return       A node representing 
            */
			std::unique_ptr<Expr> ParseBinaryOpExpr(std::unique_ptr<Expr>&& lhs);

        private:
            Lexer mLexer;                               /*!< Lexer used by the parser to acquire tokens */
            Lexer::Token mCurrentToken;                 /*!< Current token being treated by the parser */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the parser */
        };
    }
}

#endif // PARSER_H__TOSTITOS
