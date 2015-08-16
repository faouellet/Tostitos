#include "parser.h"

std::unique_ptr<ASTNode> Parser::ParseProgram(const std::string& filename)
{
    // TODO: At which level file type validation should be done?
    if (!mLexer.Init(filename))
        return nullptr;

    return ParseProgramDecl();
}

std::unique_ptr<ProgramDecl> Parser::ParseProgramDecl()
{
    std::unique_ptr<ProgramDecl> programNode = std::make_unique<ProgramDecl>();
    Lexer::Token currentToken = mLexer.GetNextToken();
    
    // TODO: Error handling when encountering an unknown token
    while (currentToken != Lexer::TOK_EOF)
    {
        switch (currentToken)
        {
        case Lexer::VAR:
            programNode->AddChildNode(ParseVarDecl());
            currentToken = mLexer.GetNextToken();
            break;
        case Lexer::SEMI_COLON:
            currentToken = mLexer.GetNextToken();
            break;
        default:
            break;
        }
    }

    return programNode;
}

std::unique_ptr<VarDecl> Parser::ParseVarDecl()
{
    // TODO: Handle possible errors at each level of the parsing of a variable
    // TODO: Is there no other way to parse a variable expression?
    if (mLexer.GetNextToken() == Lexer::IDENTIFIER)
    {
        std::unique_ptr<VarDecl> node = std::make_unique<VarDecl>(mLexer.GetCurrentStr());
        if (mLexer.GetNextToken() == Lexer::COLON)
        {
            if (mLexer.GetNextToken() == Lexer::TYPE)
            {
                if (mLexer.GetNextToken() == Lexer::EQUAL)   // Variable declaration with initialization
                {
                    switch (mLexer.GetNextToken())
                    {
                    case Lexer::FALSE:
                        node->AddValue(std::make_unique<BooleanExpr>(false));
                        break;
                    case Lexer::TRUE:
                        node->AddValue(std::make_unique<BooleanExpr>(true));
                        break;
                    case Lexer::NUMBER:
                        node->AddValue(std::make_unique<NumberExpr>(mLexer.GetCurrentNumber()));
                        break;
                    default:
                        // Error
                        break;
                    }
                }

                return node;
            }
        }
    }
    return nullptr;
}
