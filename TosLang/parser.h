#ifndef PARSER_H__TOSTITOS
#define PARSER_H__TOSTITOS

#include "lexer.h"

#include <memory>

class ASTNode;

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
    * \fn               ParseProgram
    * \brief            Generate an AST from
    * \param filename   Name of a TosLang file
    * \return           Root node of the AST
    */
    std::unique_ptr<ASTNode> ParseProgram(const std::string& filename);

private:
    /*
    * \fn       ParseProgramDecl
    * \brief    programdecl 
    *                   ::= decls
    *                   ::= stmts
    * \return   A node representing a program
    */
    std::unique_ptr<ASTNode> ParseProgramDecl();

    /*
    * \fn       ParseVarDecl
    * \brief    vardecl ::= 'var' identifierexpr ':' typeexpr ( '=' expr ) ';'
    * \return   A node representing a variable
    */
    std::unique_ptr<ASTNode> ParseVarDecl();

private:
    Lexer mLexer;               /*!< Lexer used by the parser to acquire tokens */
    Lexer::Token mCurrentToken; /*!< Current token being treated by the parser */
};

#endif // PARSER_H__TOSTITOS
