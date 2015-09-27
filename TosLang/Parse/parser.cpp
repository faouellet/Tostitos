#include "parser.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../Utils/errorlogger.h"

#include "opcodes.h"
#include "symboltable.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

static Opcode TokenToOpcode(Lexer::Token tok);

std::unique_ptr<ASTNode> Parser::ParseProgram(const std::string& filename)
{
    if (filename.substr(filename.size() - 4) != ".tos")
    {
        ErrorLogger::PrintError(ErrorLogger::ErrorType::WRONG_FILE_TYPE);
        return nullptr;
    }

    if (!mLexer.Init(filename))
    {
        ErrorLogger::PrintError(ErrorLogger::ErrorType::ERROR_OPENING_FILE);
        return nullptr;
    }

    return ParseProgramDecl();
}

std::unique_ptr<ASTNode> Parser::ParseProgramDecl()
{
    auto programNode = std::make_unique<ProgramDecl>();
    mCurrentToken = mLexer.GetNextToken();
    
    // TODO: Error handling when encountering an unknown token
    while (mCurrentToken != Lexer::Token::TOK_EOF)
    {
        switch (mCurrentToken)
        {
        case Lexer::Token::VAR:
            {
                std::unique_ptr<ASTNode> node = ParseVarDecl();

                // If there was an error in parsing the variable declaration, we skip everything until the next statement
                if (node->GetKind() == ASTNode::NodeKind::ERROR)
                    while (mCurrentToken != Lexer::Token::SEMI_COLON && mCurrentToken != Lexer::Token::TOK_EOF)
                        mCurrentToken = mLexer.GetNextToken();

                programNode->AddChildNode(std::move(node));
            }
            break;
        case Lexer::Token::SEMI_COLON:
            {
                mCurrentToken = mLexer.GetNextToken();
            }
            break;
        case Lexer::Token::IDENTIFIER:
            break; // TODO
        default:
            {
                std::unique_ptr<Expr> node = ParseExpr();

                // If there was an error in parsing the variable declaration, we skip everything until the next statement
                if (node->GetKind() == ASTNode::NodeKind::ERROR)
                    while (mCurrentToken != Lexer::Token::SEMI_COLON && mCurrentToken != Lexer::Token::TOK_EOF)
                        mCurrentToken = mLexer.GetNextToken();

                programNode->AddChildNode(std::move(node));
            }
            break;
        }
    }

    return std::move(programNode);
}

std::unique_ptr<ASTNode> Parser::ParseVarDecl()
{
    auto node = std::make_unique<ASTNode>();
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::IDENTIFIER)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }

    std::string varName = mLexer.GetCurrentStr();
    
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::COLON)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_COLON, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }

    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::TYPE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_TYPE, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        return std::move(node);
    }
     
    VarDecl* vDecl = new VarDecl(varName);
    if (!mSymbolTable->AddSymbol(varName, mLexer.GetCurrentStr() == "Int" ? Symbol(INT) : Symbol(BOOL)))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_REDEFINITION, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
		delete vDecl;
        return std::move(node);
    }

    mCurrentToken = mLexer.GetNextToken();

    if (mCurrentToken == Lexer::Token::EQUAL)   // Variable declaration with initialization
        vDecl->AddInitialization(ParseExpr());    

    if (mCurrentToken == Lexer::Token::SEMI_COLON)
    {
        node.reset(vDecl);
        return std::move(node);
    }
    else
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLine(), mLexer.GetCurrentColumn());
        delete vDecl;
        return std::move(node);
    }
}

std::unique_ptr<Expr> Parser::ParseExpr()
{
    std::unique_ptr<Expr> node;
    mCurrentToken = mLexer.GetNextToken();
    switch (mCurrentToken)
    {
    case Lexer::Token::FALSE:
        node = std::make_unique<BooleanExpr>(false);
        break;
    case Lexer::Token::TRUE:
        node = std::make_unique<BooleanExpr>(true);
        break;
    case Lexer::Token::NUMBER:
        node = std::make_unique<NumberExpr>(mLexer.GetCurrentNumber());
        break;
    case Lexer::Token::IDENTIFIER:
        node = std::make_unique<IdentifierExpr>(mLexer.GetCurrentStr());
        break;
    default:    // TODO: This should be logged
        node = nullptr;
        break;
    }

    return ParseBinaryOpExpr(std::move(node));
}

std::unique_ptr<Expr> Parser::ParseBinaryOpExpr(std::unique_ptr<Expr>&& lhs)
{
    mCurrentToken = mLexer.GetNextToken();
    if (mCurrentToken == Lexer::Token::SEMI_COLON)
    {
        return std::move(lhs);
    }
    else if (mCurrentToken < Lexer::Token::OP_START || Lexer::Token::OP_END < mCurrentToken)
    {
        // TODO: This is an error that should be logged
        return nullptr;
    }
    else
    {
        return std::make_unique<BinaryOpExpr>(TokenToOpcode(mCurrentToken), std::move(lhs), ParseExpr());
    }
}

//////////////////// Static functions ////////////////////
static Opcode TokenToOpcode(Lexer::Token tok)
{
    switch (tok)
    {
    case TosLang::FrontEnd::Lexer::Token::AND_BOOL:
        return Opcode::AND_BOOL;
    case TosLang::FrontEnd::Lexer::Token::AND_INT:
        return Opcode::AND_INT;
    case TosLang::FrontEnd::Lexer::Token::DIVIDE:
        return Opcode::DIVIDE;
    case TosLang::FrontEnd::Lexer::Token::EQUAL:
        return Opcode::EQUAL;
    case TosLang::FrontEnd::Lexer::Token::GREATER_THAN:
        return Opcode::GREATER_THAN;
    case TosLang::FrontEnd::Lexer::Token::LEFT_SHIFT:
        return Opcode::LEFT_SHIFT;
    case TosLang::FrontEnd::Lexer::Token::LESS_THAN:
        return Opcode::LESS_THAN;
    case TosLang::FrontEnd::Lexer::Token::MINUS:
        return Opcode::MINUS;
    case TosLang::FrontEnd::Lexer::Token::MODULO:
        return Opcode::MODULO;
    case TosLang::FrontEnd::Lexer::Token::MULT:
        return Opcode::MULT;
    case TosLang::FrontEnd::Lexer::Token::NOT:
        return Opcode::NOT;
    case TosLang::FrontEnd::Lexer::Token::OR_BOOL:
        return Opcode::OR_BOOL;
    case TosLang::FrontEnd::Lexer::Token::OR_INT:
        return Opcode::OR_INT;
    case TosLang::FrontEnd::Lexer::Token::PLUS:
        return Opcode::PLUS;
    case TosLang::FrontEnd::Lexer::Token::RIGHT_SHIFT:
        return Opcode::RIGHT_SHIFT;
    default:
        return Opcode::UNKNOWN_OP;
    }
}
