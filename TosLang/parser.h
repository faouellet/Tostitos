#ifndef PARSER_H__TOSTITOS
#define PARSER_H__TOSTITOS

#include "declarations.h"
#include "expressions.h"
#include "lexer.h"
#include "statements.h"

/*
* \class Parser
* \brief Part of the TosLang compiler used to generate an AST
*/
class Parser
{
public:
    Parser() = default;
    ~Parser() = default;

    /*
    * \fn ParseProgram
    * \brief Generate an AST from
    * \param filename Name of a TosLang file
    * \return Root node of the AST
    */
    std::unique_ptr<ASTNode> ParseProgram(const std::string& filename);

private:
    /*
    * \fn ParseProgramDecl
    * \brief programdecl 
    *           ::= decls
    *           ::= stmts
    * \return A node representing a program
    */
    std::unique_ptr<ProgramDecl> ParseProgramDecl();

    /*
    * \fn ParseVarDecl
    * \brief vardecl ::= 'var' identifierexpr ':' typeexpr ( '=' expr ) ';'
    * \return A node representing a variable
    */
    std::unique_ptr<VarDecl> ParseVarDecl();

private:
    Lexer mLexer;   /*!< Lexer used by the parser to acquire tokens */
};

#endif // PARSER_H__TOSTITOS
