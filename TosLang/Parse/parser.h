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
        class CompoundStmt;
        class Expr;
        class FunctionDecl;
        class IfStmt;
        class ReturnStmt;
        class VarDecl;
        class WhileStmt;
        
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

        private:    // Helpers
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
            * \brief    functiondecl ::= 'fn' identifierexpr '(' ( vardecl, )* ')' '->' typeexpr compoundstmt
            * \return   A node representing a function declaration or definition
            */
            std::unique_ptr<FunctionDecl> ParseFunctionDecl();

            /*
            * \fn       ParseVarDecl
            * \brief    vardecl ::= 'var' identifierexpr ':' typeexpr ( '=' expr )? ';'
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
			* \return       A node representing a binary expression 
            */
			std::unique_ptr<Expr> ParseBinaryOpExpr(std::unique_ptr<Expr>&& lhs);

            /*
            * \fn           ParseIdentifierExpr
            * \brief        identifierexpr 
            *                           ::= identifier
            *                           ::= identifier '(' expr* ')'
            * \return       A node representing an identifier expression or a call expression
            */
            std::unique_ptr<Expr> ParseIdentifierExpr();

        private:    // Statements
            /*
            * \fn           ParseCompoundStmt
            * \brief        '{' stmt* '}'
            * \return       A node representing a compound statement i.e. an aggregation of statements
            */
            std::unique_ptr<CompoundStmt> ParseCompoundStmt();

            /*
            * \fn           ParseIfStmt
            * \brief        'if' expr '{' compoundstmt '}' 'else' '{' compoundstmt '}'
            * \return       A node representing an if statement
            */
            std::unique_ptr<IfStmt> ParseIfStmt();

            /*
            * \fn           ParseReturnStmt
            * \brief        'return' expr? ';'
            * \return       A node representing a return statement
            */
            std::unique_ptr<ReturnStmt> ParseReturnStmt();

            /*
            * \fn           ParseIdentifierExpr
            * \brief        'while' expr '{' compoundstmt '}'
            * \return       A node representing a while statement
            */
            std::unique_ptr<WhileStmt> ParseWhileStmt();

        private:
            Lexer mLexer;                               /*!< Lexer used by the parser to acquire tokens */
            Lexer::Token mCurrentToken;                 /*!< Current token being treated by the parser */
            std::shared_ptr<SymbolTable> mSymbolTable;  /*!< Symbol table to be filled by the parser */
        };
    }
}

#endif // PARSER_H__TOSTITOS
