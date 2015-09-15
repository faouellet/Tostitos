#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE LexerError
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "frontend_error_fixture.h"

#include "Parse/lexer.h"

BOOST_FIXTURE_TEST_CASE( LexerBadInitTest, FrontEndErrorFixture )
{
    Lexer lex;
    BOOST_REQUIRE(!lex.Init("BadFile.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 0);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 0);
}

BOOST_FIXTURE_TEST_CASE( LexerBadStrLitTest, FrontEndErrorFixture )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../inputs/bad_string_literal.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLine(), 1);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentColumn(), 1);

    // var MyVar: String = "Hello;
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::VAR);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyVar");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::COLON);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::EQUAL);
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::UNKNOWN);

    // End of file
    BOOST_REQUIRE_EQUAL(lex.GetNextToken(), Lexer::Token::TOK_EOF);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Newline in string literal at line 1, column 16");
}
