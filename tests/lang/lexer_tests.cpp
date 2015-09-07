#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Lexer
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "Parse/lexer.h"

using namespace TosLang::FrontEnd;

BOOST_AUTO_TEST_CASE( InitTest )
{
    Lexer lex;
    BOOST_REQUIRE(!lex.Init("BadFile.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 0);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 0);

	BOOST_REQUIRE(lex.Init("../inputs/var_init_bool.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);
}

BOOST_AUTO_TEST_CASE( VarDeclTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../inputs/vardecl.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

    // var MyIntVar : Int;
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    //var MyBoolVar : Bool;    
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyBoolVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    // End of file
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( VarInitBoolTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../inputs/var_init_bool.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

    //var MyTrueVar : Bool = True;    
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyTrueVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TRUE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    //var MyFalseVar : Bool = False;
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyFalseVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::FALSE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    // End of file
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( VarInitIntTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../inputs/var_init_int.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

	// var MyIntVar : Int = 42;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 42);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);
}

BOOST_AUTO_TEST_CASE( VarInitIdentifierTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../inputs/var_init_identifier.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

	// var MyIntVar : Int = 42;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 42);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

	// var MyIntVar2 : Int = MyIntVar;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar2");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);
}

BOOST_AUTO_TEST_CASE( VarInitBinOpTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../inputs/binary_op.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

	// var V1 : Int = 1 + 1;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V1");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::PLUS);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

	// var V2 : Int = 1 - 1;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V2");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::MINUS);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

	// var V3 : Int = 1 * 1;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V3");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::MULT);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

	// var V4 : Int = 1 / 1;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V4");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::DIVIDE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

	// var V5 : Int = 1 % 1;
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V5");
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::MODULO);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);
}
