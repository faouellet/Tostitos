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

BOOST_AUTO_TEST_SUITE( ParseTestSuite )

BOOST_AUTO_TEST_CASE( LexerInitTest )
{
    Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/var_init_bool.tos"));
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);
}

BOOST_AUTO_TEST_CASE( LexerVarDeclTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/var/var_decl.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    // var MyIntVar: Int;
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Int");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    // var MyBoolVar: Bool;    
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyBoolVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Bool");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var MyStringVar: String;    
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyStringVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var MyTabVar[10]: Int;
	/*BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyTabVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACKET);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 10);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACKET);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Int");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);*/

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitBoolTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/var/var_init_bool.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    //var MyTrueVar : Bool = True;    
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyTrueVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    //var MyFalseVar : Bool = False;
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyFalseVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FALSE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitIntTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/var_init_int.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var MyIntVar : Int = 42;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 42);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitIdentifierTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/var_init_identifier.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var MyIntVar : Int = 42;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 42);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var MyIntVar2 : Int = MyIntVar;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitStringTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/var_init_string.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var MyStringVar: String = "Hello World";
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyStringVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::STRING_LITERAL);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Hello World");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);
	
	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitBinOpIntTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/binary_op_int.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var V1 : Int = 1 + 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V1");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::PLUS);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V2 : Int = 1 - 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::MINUS);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V3 : Int = 1 * 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V3");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::MULT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V4 : Int = 1 / 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V4");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::DIVIDE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V5 : Int = 1 % 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V5");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::MODULO);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V6 : Int = 1 & 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V6");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::AND_INT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V7 : Int = 1 | 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V7");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::OR_INT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V8 : Int = 1 >> 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V8");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_SHIFT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V9 : Int = 1 % 1;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V9");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_SHIFT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);
	
	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitBinOpBoolTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/binary_op_bool.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var V1 : Bool = True && False;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V1");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::AND_BOOL);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FALSE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V2 : Bool = True || False;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::OR_BOOL);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FALSE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V3 : Bool = 1 > 2;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V3");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::GREATER_THAN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// var V4 : Bool = 1 < 2;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "V4");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LESS_THAN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerVarInitUnaryOpTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/var/unary_op.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// var MyBool: Bool = !True
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyBool");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NOT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
}

BOOST_AUTO_TEST_CASE( LexerFunctionDefinitionTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/function/fn_def_multi_args.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// fn MyFunc(arg1: Int, arg2 : Int, arg3 : Int) -> Int {
	//     return arg1 + arg2 + arg3;
	// }

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FUNCTION);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyFunc");
	
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg1");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COMMA);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);

    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COMMA);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg3");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ARROW);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RETURN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg1");

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::PLUS);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg2");

    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::PLUS);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg3");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerIfTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/if/if_simple_cond.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    // fn main() -> Void {
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FUNCTION);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "main");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ARROW);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);

	// var MyBool: Bool = True
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "BoolVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// if BoolVar {
	//     BoolVar = False;
	// }
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IF);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "BoolVar");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FALSE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);
    
    // End of function
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);
	
	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerWhileTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/while/while_simple_cond.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    // fn main() -> Void {
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FUNCTION);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "main");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ARROW);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);

    // var MyBool: Bool = True
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "BoolVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TRUE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    // while BoolVar {
    //     BoolVar = False;
    // }
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::WHILE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "BoolVar");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FALSE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);

    // End of function
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerIOTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/io/io.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    // fn main() -> Void {
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FUNCTION);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "main");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ARROW);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);

	// var Message: String;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Message");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// scan Message;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SCAN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Message");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// print Message;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::PRINT);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "Message");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    // }
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerThreadTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/thread.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	// fn MyFunc(arg1: Int, arg2 : Int) -> Int {
	//     return arg1 + arg2;
	// }

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::FUNCTION);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyFunc");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg1");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COMMA);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ARROW);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_BRACE);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RETURN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg1");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::PLUS);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "arg2");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_BRACE);

	// spawn MyFunc(1,2);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SPAWN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyFunc");
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::LEFT_PAREN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 1);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COMMA);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::NUMBER);
	BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 2);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::RIGHT_PAREN);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// sync;
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SYNC);
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerCommentTest )
{
	Lexer lex;
	BOOST_REQUIRE(lex.Init("../sources/comment/comment.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COMMENT);

	// End of file
	BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_CASE( LexerMLCommentTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/comment/ml_comment.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1);

    // /* This is an unclosed multiline comment
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ML_COMMENT);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);
}

BOOST_AUTO_TEST_SUITE_END()
