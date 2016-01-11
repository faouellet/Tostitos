#include "parser.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include "../Common/opcodes.h"

#include "../Utils/errorlogger.h"

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
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
    
    std::unique_ptr<Decl> node = std::make_unique<Decl>();
    while (mCurrentToken != Lexer::Token::TOK_EOF)
    {
        // Only function and variable are allowed at the program top level
        switch (mCurrentToken)
        {
        case Lexer::Token::VAR:
            node.reset(ParseVarDecl().release());
            break;
        case Lexer::Token::FUNCTION:
             node.reset(ParseFunctionDecl().release());
            break;
        default:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
        }
        
        programNode->AddProgramStmt(std::move(node));

        // Go to next declaration.
        // In case an error happens, this will skip straight to the next declaration
        while ((mCurrentToken != Lexer::Token::VAR) 
            && (mCurrentToken != Lexer::Token::FUNCTION) 
            && (mCurrentToken != Lexer::Token::TOK_EOF))
            mCurrentToken = mLexer.GetNextToken();
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
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_LEFT_PAREN, mLexer.GetCurrentLocation());
        return std::move(fnNode);
    }

    // Parse all the parameters
    auto params = std::make_unique<ParamVarDecls>();
    std::unique_ptr<VarDecl> param;
    std::string varName;
    while ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::RIGHT_PAREN)
    {        
        if (mCurrentToken != Lexer::Token::IDENTIFIER)
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PARAM_MISSING_NAME, mLexer.GetCurrentLocation());
            return std::move(fnNode);
        }
        varName = mLexer.GetCurrentStr();

        if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::COLON)
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PARAM_MISSING_COLON, mLexer.GetCurrentLocation());
            return std::move(fnNode);
        }

        if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::TYPE)
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PARAM_MISSING_TYPE, mLexer.GetCurrentLocation());
            return std::move(fnNode);
        }
        param.reset(std::make_unique<VarDecl>(varName, mLexer.GetCurrentType()).release());

        mCurrentToken = mLexer.GetNextToken();
        if ((mCurrentToken != Lexer::Token::RIGHT_PAREN) && (mCurrentToken != Lexer::Token::COMMA))
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_RIGHT_PAREN, mLexer.GetCurrentLocation());
            return std::move(fnNode);
        }

        params->AddParameter(std::move(param));

        if (mCurrentToken == Lexer::Token::RIGHT_PAREN)
            break;  // TODO: Refactor the loop so it won't be needed
    }

    // Parse the return type
    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::ARROW)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_ARROW, mLexer.GetCurrentLocation());
        return std::move(fnNode);
    }

    if ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::TYPE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_MISSING_RETURN_TYPE, mLexer.GetCurrentLocation());
        return std::move(fnNode);
    }
    Common::Type fnType = mLexer.GetCurrentType();

    // Parse the function body
    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<CompoundStmt> body = ParseCompoundStmt();
    // TODO: Check if the body is not null. If it is, we need to log an error
    fnNode.reset(new FunctionDecl(fnName, fnType, std::move(params), std::move(body)));
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
    Common::Type vType = mLexer.GetCurrentType();
    if (vType == Common::Type::VOID)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::VAR_VOID_TYPE, mLexer.GetCurrentLocation());
        return std::move(node);
    }
     
    VarDecl* vDecl = new VarDecl(varName, vType);
    
    mCurrentToken = mLexer.GetNextToken();

    // Variable declaration with initialization
    if (mCurrentToken == Lexer::Token::EQUAL)
    {
        mCurrentToken = mLexer.GetNextToken();
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

    // Parse the beginning of the expression
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
    case Lexer::Token::SEMI_COLON:
        return nullptr;
    default:
        if ((Lexer::Token::OP_START <= mCurrentToken) && (mCurrentToken <= Lexer::Token::OP_END))
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_USE_OPERATION, mLexer.GetCurrentLocation());
            mCurrentToken = mLexer.GetNextToken();
        }
        return nullptr;
    }

    // Look what comes next
    mCurrentToken = mLexer.GetNextToken();
    
    // Here, what could happen is one of the following cases:
    //      1- The expression could be part of a binary expression
    //      2- The expression could be a function call
    //      3- The expression is done being parsed
    if ((Lexer::Token::OP_START <= mCurrentToken) && (mCurrentToken <= Lexer::Token::OP_END))
        return ParseBinaryOpExpr(mCurrentToken, std::move(node));
    else if(mCurrentToken == Lexer::Token::LEFT_PAREN)
        return ParseCallExpr(std::move(node));
    else if ((mCurrentToken == Lexer::Token::SEMI_COLON) 
        || (mCurrentToken == Lexer::Token::LEFT_BRACE)
        || (mCurrentToken == Lexer::Token::RIGHT_BRACE)
        || (mCurrentToken == Lexer::Token::RIGHT_PAREN)
        || (mCurrentToken == Lexer::Token::COMMA))
        return std::move(node);

    // The expression has a second part and isn't well-formed, log an error
    ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_OPERATION, mLexer.GetCurrentLocation());
    return nullptr;
}

std::unique_ptr<Expr> Parser::ParseBinaryOpExpr(Lexer::Token op, std::unique_ptr<Expr>&& lhs)
{
    // Parse the right-hand side of the expression. It is assumed that the left-hand side of the expression has been parsed.

    mCurrentToken = mLexer.GetNextToken();

    std::unique_ptr<Expr> rhs = ParseExpr();
    if (rhs == nullptr)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_RHS, mLexer.GetCurrentLocation());
        return nullptr;
    }
    else
    {
        return std::make_unique<BinaryOpExpr>(TokenToOpcode(op), std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<Expr> Parser::ParseCallExpr(std::unique_ptr<Expr>&& fn)
{
    std::unique_ptr<Expr> cExpr;
    std::vector<std::unique_ptr<Expr>> args;
    std::unique_ptr<Expr> arg;

    mCurrentToken = mLexer.GetNextToken();

    // Parse the expression the function is being called with
    while (mCurrentToken != Lexer::Token::RIGHT_PAREN)
    {
        // TODO: Check if the argument is null. If it is, log an error
        arg.reset(ParseExpr().release());
        args.push_back(std::move(arg));

        if (mCurrentToken == Lexer::Token::RIGHT_PAREN) // We're done
        {
            break;
        }
        else if (mCurrentToken == Lexer::Token::COMMA)  // We have another parameter still
        {
            mCurrentToken = mLexer.GetNextToken();
        }
        else if (mCurrentToken == Lexer::Token::SEMI_COLON) // The closing parenthesis is missing
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_MISSING_PAREN, mLexer.GetCurrentLocation());
            cExpr.reset();
            return std::move(cExpr);
        }
        else // An argument isn't well formed
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::CALL_ARG_ERROR, mLexer.GetCurrentLocation());
            cExpr.reset();
            return std::move(cExpr);
        }
    }

    mCurrentToken = mLexer.GetNextToken();

    cExpr.reset(new CallExpr(fn->GetName(), std::move(args)));

    return cExpr;
}

std::unique_ptr<CompoundStmt> Parser::ParseCompoundStmt()
{
    // The compound statement should begin with a left brace
    if (mCurrentToken != Lexer::Token::LEFT_BRACE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SYNTAX_MISSING_LBRACE, mLexer.GetCurrentLocation());
        return nullptr;
    }

    auto cStmt = std::make_unique<CompoundStmt>();

    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<ASTNode> node;

    while ((mCurrentToken != Lexer::Token::RIGHT_BRACE) && (mCurrentToken != Lexer::Token::TOK_EOF))
    {
        switch (mCurrentToken)
        {
        case Lexer::Token::VAR:
            node.reset(ParseVarDecl().release());
            break;
        case Lexer::Token::IDENTIFIER:
            node.reset(ParseExpr().release());
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
        case Lexer::Token::PRINT:
            node.reset(ParsePrintStmt().release());
            break;
        case Lexer::Token::SCAN:
            node.reset(ParseScanStmt().release());
            break;
        case Lexer::Token::FUNCTION:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::FN_INTERNAL, mLexer.GetCurrentLocation());
            // We skip everything until either the end of the internal function, the end of the function currently
            // being parsed or the end of the program file
            while ((mCurrentToken != Lexer::Token::RIGHT_BRACE) && (mCurrentToken != Lexer::Token::TOK_EOF))
                mCurrentToken = mLexer.GetNextToken();
            break;
            // TODO: No internal compound statement
        default:
            break;
        }

        cStmt->AddStatement(std::move(node));

        mCurrentToken = mLexer.GetNextToken();
    }

    // Check that the compound statement is closed correctly
    if (mCurrentToken != Lexer::Token::RIGHT_BRACE)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SYNTAX_MISSING_RBRACE, mLexer.GetCurrentLocation());
        return std::move(cStmt);
    }

    return cStmt;
}

std::unique_ptr<IfStmt> Parser::ParseIfStmt()
{
    std::unique_ptr<IfStmt> ifStmt = std::make_unique<IfStmt>();
    
    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<Expr> condExpr = ParseExpr();
    if (condExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::IF_MISSING_COND, mLexer.GetCurrentLocation());

    std::unique_ptr<CompoundStmt> thenStmt = ParseCompoundStmt();
    if (thenStmt == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::IF_MISSING_BODY, mLexer.GetCurrentLocation());
        
    if ((condExpr != nullptr) && (thenStmt != nullptr))
        ifStmt.reset(new IfStmt(std::move(condExpr), std::move(thenStmt)));

    return ifStmt;
}

std::unique_ptr<PrintStmt> Parser::ParsePrintStmt()
{
    std::unique_ptr<PrintStmt> pStmt = std::make_unique<PrintStmt>();

    mCurrentToken = mLexer.GetNextToken();

    switch (mCurrentToken)
    {
    case Lexer::Token::IDENTIFIER:
        pStmt->AddMessage(std::make_unique<IdentifierExpr>(mLexer.GetCurrentStr()));
        break;
    case Lexer::Token::NUMBER:
        pStmt->AddMessage(std::make_unique<NumberExpr>(mLexer.GetCurrentNumber()));
        break;
    case Lexer::Token::STRING_LITERAL:
        pStmt->AddMessage(std::make_unique<IdentifierExpr>(mLexer.GetCurrentStr()));
        break;
    case Lexer::Token::SEMI_COLON:
        break;
    default:
        // TODO: Error should be logged in the type checker instead
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PRINT_WRONG_MSG, mLexer.GetCurrentLocation());
        break;
    }

    mCurrentToken = mLexer.GetNextToken();

    return pStmt;
}

std::unique_ptr<ReturnStmt> Parser::ParseReturnStmt()
{
    auto rStmt = std::make_unique<ReturnStmt>();

    mCurrentToken = mLexer.GetNextToken();
    rStmt->AddReturnValue(std::move(ParseExpr()));

    // The return statement must be terminated by a semi-colon
    if (mCurrentToken != Lexer::Token::SEMI_COLON)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLocation());
    
    return rStmt;
}

std::unique_ptr<ScanStmt> Parser::ParseScanStmt()
{
    std::unique_ptr<ScanStmt> sStmt = std::make_unique<ScanStmt>();

    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<Expr> inputExpr = ParseExpr();

    if (inputExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SCAN_MISSING_INPUT_VAR, mLexer.GetCurrentLocation());
    else
        sStmt.reset(new ScanStmt(std::make_unique<IdentifierExpr>(inputExpr->GetName())));
        
    return sStmt;
}

std::unique_ptr<WhileStmt> Parser::ParseWhileStmt()
{
    std::unique_ptr<WhileStmt> whileStmt = std::make_unique<WhileStmt>();

    mCurrentToken = mLexer.GetNextToken();
    std::unique_ptr<Expr> condExpr = ParseExpr();
    if (condExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WHILE_MISSING_COND, mLexer.GetCurrentLocation());

    std::unique_ptr<CompoundStmt> body = ParseCompoundStmt();
    if (body == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WHILE_MISSING_BODY, mLexer.GetCurrentLocation());

    if ((condExpr != nullptr) && (body != nullptr))
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
