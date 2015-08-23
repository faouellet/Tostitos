#include "parser.h"

#include <iostream>

std::unique_ptr<ASTNode> Parser::ParseProgram(const std::string& filename)
{
    if (filename.substr(filename.size() - 4) != ".tos")
    {
        std::cerr << "FILE ERROR: Wrong file type" << std::endl;
        return nullptr;
    }

    if (!mLexer.Init(filename))
    {
        std::cerr << "FILE ERROR: Problem opening the specified file" << std::endl;
        return nullptr;
    }

    return ParseProgramDecl();
}

std::unique_ptr<ASTNode> Parser::ParseProgramDecl()
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

    return std::move(programNode);
}

std::unique_ptr<ASTNode> Parser::ParseVarDecl()
{
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

                return std::move(node);
            }
            else
            {
                std::cerr << "VAR ERROR: Missing type from variable declaration" << std::endl;
            }
        }
        else
        {
            std::cerr << "VAR ERROR: Missing : between a variable and its type" << std::endl;
        }
    }
    else
    {
        std::cerr << "VAR ERROR: The var keyword should be followed by an identifier" << std::endl;
    }

    return std::make_unique<ASTNode>();
}
