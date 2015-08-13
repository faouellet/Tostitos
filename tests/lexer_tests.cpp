#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Lexer
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "lexer.h"

BOOST_AUTO_TEST_CASE( InitTest )
{
    Lexer lex;
    BOOST_REQUIRE(!lex.Init("BadFile.tos"));
    BOOST_REQUIRE(lex.Init("../inputs/var.tos"));
}

BOOST_AUTO_TEST_CASE( GetNextTokenVarTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../inputs/var.tos"));

    // var MyIntVar : Int = 42;
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyIntVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::INT);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::NUMBER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentNumber(), 42);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    //var MyBoolVar : Bool = True;    
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyBoolVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::BOOL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TRUE);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::SEMI_COLON);

    // End of file
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TOK_EOF);
}
