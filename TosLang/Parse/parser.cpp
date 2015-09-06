#include "parser.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../Utils/errorlogger.h"

#include "symboltable.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

std::unique_ptr<ASTNode> Parser::ParseProgram(const std::string& filename)
{
    if (filename.substr(filename.size() - 4) != ".tos")
    {
        ErrorLogger::PrintError(ErrorLogger::WRONG_FILE_TYPE);
        return nullptr;
    }

    if (!mLexer.Init(filename))
    {
        ErrorLogger::PrintError(ErrorLogger::ERROR_OPENING_FILE);
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
    std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>();
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::IDENTIFIER)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }

    std::string varName = mLexer.GetCurrentStr();
    
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::COLON)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::VAR_MISSING_COLON, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }

    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::TYPE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::VAR_MISSING_TYPE, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }
     
    VarDecl* vDecl = new VarDecl(varName);
    if (!mSymbolTable->AddSymbol(varName, mLexer.GetCurrentStr() == "Int" ? Symbol(INT) : Symbol(BOOL)))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::VAR_REDEFINITION, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
		delete vDecl;
        return std::move(node);
    }

    mCurrentToken = mLexer.GetNextToken();

    if (mCurrentToken == Lexer::EQUAL)   // Variable declaration with initialization
    {
        switch (mLexer.GetNextToken())
        {
        case Lexer::FALSE:
            vDecl->AddValue(std::make_unique<BooleanExpr>(false));
            break;
        case Lexer::TRUE:
            vDecl->AddValue(std::make_unique<BooleanExpr>(true));
            break;
        case Lexer::NUMBER:
            vDecl->AddValue(std::make_unique<NumberExpr>(mLexer.GetCurrentNumber()));
            mSymbolTable->AddSymbol(mLexer.GetCurrentNumber());
            break;
        case Lexer::IDENTIFIER:
            vDecl->AddValue(std::make_unique<IdentifierExpr>(mLexer.GetCurrentStr()));
        default:
            break;
        }

        mCurrentToken = mLexer.GetNextToken();
    }
    

    if (mCurrentToken == Lexer::SEMI_COLON)
    {
        node.reset(vDecl);
        return std::move(node);
    }
    else
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::MISSING_SEMI_COLON, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        delete vDecl;
        return std::move(node);
    }
}
