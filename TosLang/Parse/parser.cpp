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

void Parser::GoToNextStmt()
{
    while (mCurrentToken != Lexer::Token::SEMI_COLON && mCurrentToken != Lexer::Token::TOK_EOF)
        mCurrentToken = mLexer.GetNextToken();
}

std::unique_ptr<ASTNode> Parser::ParseProgramDecl()
{
    auto programNode = std::make_unique<ProgramDecl>();
    mCurrentToken = mLexer.GetNextToken();
    
    // TODO: Error handling when encountering an unknown token
    std::unique_ptr<Decl> node;
    while (mCurrentToken != Lexer::Token::TOK_EOF)
    {
        switch (mCurrentToken)
        {
        case Lexer::Token::VAR:
            node.reset(ParseVarDecl().release());
            break;
        case Lexer::Token::FUNCTION:
             node.reset(ParseFunctionDecl().release());
            break;
        default:
            // TODO: Log an error
            break;
        }

        // If there was an error in parsing the variable declaration, we go to the next statement
        if (node->GetKind() == ASTNode::NodeKind::ERROR)
            GoToNextStmt();

        // Go to the start of the next statement
        if (mCurrentToken == Lexer::Token::SEMI_COLON)
            mCurrentToken = mLexer.GetNextToken();

        programNode->AddProgramStmt(std::move(node));
    }

    return std::move(programNode);
}

std::unique_ptr<FunctionDecl> Parser::ParseFunctionDecl()
{
    std::unique_ptr<FunctionDecl> fnNode = std::make_unique<FunctionDecl>();
    // Get the function name
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::IDENTIFIER)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        return std::move(fnNode);
    }
    std::string fnName = mLexer.GetCurrentStr();

    // Make sure the function name is followed by an opening parenthesis
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::LEFT_PAREN)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_PAREN, mLexer.GetCurrentLocation());
        return std::move(fnNode);
    }

    // Parse all the parameters
    auto params = std::make_unique<ParamVarDecls>();
    std::unique_ptr<VarDecl> param;
    while ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::RIGHT_PAREN)
    {
        param.reset(ParseVarDecl().release());
        if (param == nullptr)
        {
            return std::move(fnNode);
        }

        mCurrentToken = mLexer.GetNextToken();
        if ((mCurrentToken != Lexer::Token::RIGHT_PAREN) || (mCurrentToken != Lexer::Token::COMMA))
        {
            return std::move(fnNode);
        }

        params->AddParameter(std::move(param));
    }

    // Parse the return type
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::ARROW)
    {
        // TODO: Log an error
        return std::move(fnNode);
    }

    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::TYPE)
    {
        // TODO: Log an error
        return std::move(fnNode);
    }

    // Parse the function body
    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<CompoundStmt> body = ParseCompoundStmt();
    // TODO: Check if the body is not null. If it is, we need to log an error
    fnNode.reset(new FunctionDecl(fnName, std::move(params), std::move(body)));
    return fnNode;
}

std::unique_ptr<VarDecl> Parser::ParseVarDecl()
{
    std::unique_ptr<VarDecl> node = std::make_unique<VarDecl>();
    // Get the variable name
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::IDENTIFIER)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        return std::move(node);
    }
    std::string varName = mLexer.GetCurrentStr();
    
    // Make sure the variable name is followed by a colon
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::COLON)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_COLON, mLexer.GetCurrentLocation());
        return std::move(node);
    }

    // Get the type of the variable
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::TYPE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_TYPE, mLexer.GetCurrentLocation());
        return std::move(node);
    }
     
    // Add the variable to the symbol table
    VarDecl* vDecl = new VarDecl(varName);
    if (!mSymbolTable->AddSymbol(varName, mLexer.GetCurrentStr() == "Int" ? Symbol(INT) : Symbol(BOOL)))
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_REDEFINITION, mLexer.GetCurrentLocation());
		delete vDecl;
        return std::move(node);
    }

    mCurrentToken = mLexer.GetNextToken();

    // Variable declaration with initialization
    if (mCurrentToken == Lexer::Token::EQUAL)
    {
        if (!vDecl->AddInitialization(ParseExpr()))
        {
            delete vDecl;
            return std::move(node);
        }
    }

    // Make sure the statement is finished with a semi-colon
    if (mCurrentToken == Lexer::Token::SEMI_COLON)
    {
        node.reset(vDecl);
        return std::move(node);
    }
    else
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLocation());
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
        node = ParseIdentifierExpr();
        break;
    default: // Log the error and keep the parsing going
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_RHS, mLexer.GetCurrentLocation());
        GoToNextStmt();
        return nullptr;
    }

    // We're at a semi-colon, we're done
    if (mCurrentToken == Lexer::Token::SEMI_COLON)
        return std::move(node);

    return ParseBinaryOpExpr(std::move(node));
}

std::unique_ptr<Expr> Parser::ParseBinaryOpExpr(std::unique_ptr<Expr>&& lhs)
{
    mCurrentToken = mLexer.GetNextToken();

    // We're at a semi-colon, we're done
    if (mCurrentToken == Lexer::Token::SEMI_COLON)
    {
        return std::move(lhs);
    }
    // The current token is not a valid operation, log an error
    else if (mCurrentToken < Lexer::Token::OP_START || Lexer::Token::OP_END < mCurrentToken)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_OPERATION, mLexer.GetCurrentLocation());
        return nullptr;
    }
    else
    {
        Lexer::Token operationToken = mCurrentToken;
        std::unique_ptr<Expr> rhs = ParseExpr();
        if (rhs == nullptr)
            return nullptr;
        else
            return std::make_unique<BinaryOpExpr>(TokenToOpcode(operationToken), std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<Expr> Parser::ParseIdentifierExpr()
{
    std::unique_ptr<Expr> iExpr;
    std::string iName = mLexer.GetCurrentStr();
    mCurrentToken = mLexer.GetNextToken();

    // If the identifier is followed by an opening parenthesis, it is a function call
    if (mCurrentToken == Lexer::Token::LEFT_PAREN)
    {
        std::vector<std::unique_ptr<Expr>> args;
        std::unique_ptr<Expr> arg;

        while ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::RIGHT_PAREN)
        {
            // TODO: Check if the argument is null. If it is, log an error
            arg.reset(ParseExpr().release());

            mCurrentToken = mLexer.GetNextToken();
            if ((mCurrentToken != Lexer::Token::RIGHT_PAREN) || (mCurrentToken != Lexer::Token::COMMA))
            {
                return std::move(iExpr);
            }
        }

        iExpr.reset(new CallExpr(iName, std::move(args)));
    }
    else
    {
        iExpr.reset(new IdentifierExpr(iName));
    }

    return iExpr;
}

std::unique_ptr<CompoundStmt> Parser::ParseCompoundStmt()
{
    std::unique_ptr<CompoundStmt> cStmt;

    if (mCurrentToken != Lexer::Token::LEFT_BRACE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SYNTAX_MISSING_LBRACE, mLexer.GetCurrentLocation());
        return std::move(cStmt);
    }

    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<ASTNode> node;
    while ((mCurrentToken != Lexer::Token::RIGHT_BRACE) && (mCurrentToken != Lexer::Token::TOK_EOF))
    {
        switch (mCurrentToken)
        {
        case Lexer::Token::VAR:
            node.reset(ParseVarDecl().release());
            break;
        case Lexer::Token::IF:
            node.reset(ParseIfStmt().release());
            break;
        case Lexer::Token::RETURN:
            node.reset(ParseReturnStmt().release());
            break;
        case Lexer::Token::WHILE:
            node.reset(ParseWhileStmt().release());
            break;
        default:
            break;
        }
    }

    if (mCurrentToken != Lexer::Token::RIGHT_BRACE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SYNTAX_MISSING_RBRACE, mLexer.GetCurrentLocation());
        return std::move(cStmt);
    }

    return cStmt;
}

std::unique_ptr<IfStmt> Parser::ParseIfStmt()
{
    std::unique_ptr<IfStmt> ifStmt;
    
    // TODO: Check if they're not null. If one of them is, we need to log an error
    std::unique_ptr<Expr> condExpr = ParseExpr();
    std::unique_ptr<CompoundStmt> thenStmt = ParseCompoundStmt();
        
    ifStmt.reset(new IfStmt(std::move(condExpr), std::move(thenStmt)));
    return ifStmt;
}

std::unique_ptr<ReturnStmt> Parser::ParseReturnStmt()
{
    std::unique_ptr<ReturnStmt> rStmt;

    rStmt->AddReturnValue(std::move(ParseExpr()));

    return rStmt;
}

std::unique_ptr<WhileStmt> Parser::ParseWhileStmt()
{
    std::unique_ptr<WhileStmt> whileStmt;

    // TODO: Check if they're not null. If one of them is, we need to log an error
    std::unique_ptr<Expr> condExpr = ParseExpr();
    std::unique_ptr<CompoundStmt> body = ParseCompoundStmt();

    whileStmt.reset(new WhileStmt(std::move(condExpr), std::move(body)));
    return whileStmt;
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
