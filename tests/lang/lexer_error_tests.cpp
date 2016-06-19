#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE LexerError
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "toslang_parser_fixture.h"

#include "Parse/lexer.h"

BOOST_FIXTURE_TEST_SUITE( ParseTestSuite, TosLangParserFixture )

BOOST_AUTO_TEST_CASE( LexerBadInitTest )
{
    Lexer lex;
    BOOST_REQUIRE(!lex.Init("BadFile.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 0U);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 0U);
}

BOOST_AUTO_TEST_CASE( LexerBadStrLitTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/var/bad_string_literal.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1U);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1U);

    // var MyVar1: String = "Hel
    // lo";
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyVar1");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::UNKNOWN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::SEMI_COLON);

    // var MyVar2: String = "Hello;
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::IDENTIFIER);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "MyVar2");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::COLON);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TYPE);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentStr(), "String");
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::ASSIGN);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::UNKNOWN);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 2);
    BOOST_REQUIRE_EQUAL(messages[0], "LITERAL ERROR: Newline in string literal at line 1, column 17");
    BOOST_REQUIRE_EQUAL(messages[1], "LITERAL ERROR: Missing closing quote at line 2, column 17");
}

BOOST_AUTO_TEST_CASE( LexerBadVarNameTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/var/bad_var_name.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1U);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1U);

    // var 1Var: Int = 1;
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::VAR);
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::UNKNOWN);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 1);
    BOOST_REQUIRE_EQUAL(messages[0], "LITERAL ERROR: Bad suffix on number at line 1, column 4");
}

BOOST_AUTO_TEST_CASE( LexerBadMLCommentTest )
{
    Lexer lex;
    BOOST_REQUIRE(lex.Init("../sources/comment/bad_ml_comment.tos"));
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentLine(), 1U);
    BOOST_REQUIRE_EQUAL(lex.GetCurrentLocation().GetCurrentColumn(), 1U);

    // /* This is an unclosed multiline comment
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::UNKNOWN);

    // End of file
    BOOST_REQUIRE(lex.GetNextToken() == Lexer::Token::TOK_EOF);

    std::vector<std::string> messages{ GetErrorMessages() };

    // Check if the correct error messages got printed
    BOOST_REQUIRE_EQUAL(messages.size(), 1U);
    BOOST_REQUIRE_EQUAL(messages[0], "ERROR: Unclosed multiline comment at line 1, column 1");
}

BOOST_AUTO_TEST_SUITE_END()
