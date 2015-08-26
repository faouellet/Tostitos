#include "parser.h"

#include "declarations.h"
#include "expressions.h"

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
    mCurrentToken = mLexer.GetNextToken();
    
    // TODO: Error handling when encountering an unknown token
    while (mCurrentToken != Lexer::TOK_EOF)
    {
        switch (mCurrentToken)
        {
        case Lexer::VAR:
            {
                std::unique_ptr<ASTNode> node = ParseVarDecl();

                // If there was an error in parsing the variable declaration, we skip everything until the next statement
                if (node->GetKind() == ASTNode::ERROR)
                    while (mCurrentToken != Lexer::SEMI_COLON && mCurrentToken != Lexer::TOK_EOF)
                        mCurrentToken = mLexer.GetNextToken();

                programNode->AddChildNode(std::move(node));
            }
            break;
        case Lexer::SEMI_COLON:
            {
                mCurrentToken = mLexer.GetNextToken();
            }
            break;
        default:
            break;
        }
    }

    return std::move(programNode);
}

std::unique_ptr<ASTNode> Parser::ParseVarDecl()
{
    if ((mCurrentToken = mLexer.GetNextToken()) == Lexer::IDENTIFIER)
    {
        std::string varName = mLexer.GetCurrentStr();
        if ((mCurrentToken = mLexer.GetNextToken()) == Lexer::COLON)
        {
            if ((mCurrentToken = mLexer.GetNextToken()) == Lexer::TYPE)
            {
                std::unique_ptr<VarDecl> node = std::make_unique<VarDecl>(varName, mLexer.GetCurrentType());

                mCurrentToken = mLexer.GetNextToken();

                if (mCurrentToken == Lexer::EQUAL)   // Variable declaration with initialization
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
                        break;
                    }

                    mCurrentToken = mLexer.GetNextToken();
                }
                
                if (mCurrentToken == Lexer::SEMI_COLON)
                {
                    return std::move(node);
                }
                else
                {
                    std::cerr << "ERROR: Expected a ;" << std::endl;
                }

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
