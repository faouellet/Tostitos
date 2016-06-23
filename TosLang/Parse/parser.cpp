#include "parser.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include "../Common/opcodes.h"

#include "../Utils/errorlogger.h"

#include <algorithm>

using namespace TosLang::FrontEnd;
using namespace TosLang::Common;
using namespace TosLang::Utils;

static Operation TokenToOpcode(Lexer::Token tok);

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
        case Lexer::Token::COMMENT:
        case Lexer::Token::ML_COMMENT:
            break;
        default:
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
        }
        
        programNode->AddProgramDecl(std::move(node));

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
    const std::string fnName = mLexer.GetCurrentStr();
    SourceLocation srcLoc = mLexer.GetCurrentLocation();

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
        srcLoc = mLexer.GetCurrentLocation();
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
        param.reset(std::make_unique<VarDecl>(varName, mLexer.GetCurrentType(), /*isFunctionParam=*/true, srcLoc).release());

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
    fnNode.reset(new FunctionDecl(fnName, fnType, std::move(params), std::move(body), srcLoc));
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
    const std::string varName = mLexer.GetCurrentStr();
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();
    
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
     
    VarDecl* vDecl = new VarDecl(varName, vType, /*isFunctionParam=*/false, srcLoc);
    
    mCurrentToken = mLexer.GetNextToken();

    // Variable declaration with initialization
    if (mCurrentToken == Lexer::Token::ASSIGN)
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
        node = std::make_unique<BooleanExpr>(false, mLexer.GetCurrentLocation());
        break;
    case Lexer::Token::TRUE:
        node = std::make_unique<BooleanExpr>(true, mLexer.GetCurrentLocation());
        break;
    case Lexer::Token::NUMBER:
        node = std::make_unique<NumberExpr>(mLexer.GetCurrentNumber(), mLexer.GetCurrentLocation());
        break;
    case Lexer::Token::IDENTIFIER:
        node = std::make_unique<IdentifierExpr>(mLexer.GetCurrentStr(), mLexer.GetCurrentLocation());
        break;
    case Lexer::Token::SEMI_COLON:
        return nullptr;
    case Lexer::Token::LEFT_BRACKET:
        node = ParseArrayExpr();
        break;
    case Lexer::Token::STRING_LITERAL:
        node = std::make_unique<StringExpr>(mLexer.GetCurrentStr(), mLexer.GetCurrentLocation());
        break;
    default:
        if ((Lexer::Token::OP_START <= mCurrentToken) && (mCurrentToken <= Lexer::Token::OP_END))
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_USE_OPERATION, mLexer.GetCurrentLocation());
            mCurrentToken = mLexer.GetNextToken();
        }
        return nullptr;
    }

    // Look at what comes next
    mCurrentToken = mLexer.GetNextToken();

    const Lexer::Token exprTerminators[] = { Lexer::Token::SEMI_COLON, Lexer::Token::LEFT_BRACE, 
                                             Lexer::Token::RIGHT_BRACE, Lexer::Token::RIGHT_PAREN, Lexer::Token::COMMA };
    const auto& terminatorsBegin = std::begin(exprTerminators);
    const auto& terminatorsEnd = std::end(exprTerminators);

    // Here, what could happen is one of the following cases:
    //      1- The expression could be part of a binary expression
    //      2- The expression could be a function call
    //      3- The expression could be an indexed expression
    //      4- The expression is done being parsed
    while (std::find(terminatorsBegin, terminatorsEnd, mCurrentToken) == terminatorsEnd)
    {
        if ((Lexer::Token::OP_START <= mCurrentToken) && (mCurrentToken <= Lexer::Token::OP_END))
        {
            node.reset(ParseBinaryOpExpr(mCurrentToken, std::move(node)).release());
        }
        else if (mCurrentToken == Lexer::Token::LEFT_PAREN)
        {
            node.reset(ParseCallExpr(std::move(node)).release());
        }
        else if (mCurrentToken == Lexer::Token::LEFT_BRACKET)
        {
            // TODO: Check that it really is an identifier we're trying to index, if not log an error (also unit test this)
            std::unique_ptr<Expr> identExpr{ node.release() };
            std::unique_ptr<Expr> indexExpr = ParseExpr();
            SourceLocation arraySrcLoc = mLexer.GetCurrentLocation();

            if (mCurrentToken == Lexer::Token::RIGHT_BRACKET)
            {
                // TODO: Log an error and add a test for it
                return nullptr;
            }

            node.reset(new IndexExpr(std::move(identExpr), std::move(indexExpr), arraySrcLoc));
        }
        else
        {
            // The expression has a trailing part and isn't well-formed, log an error
            //ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_OPERATION, mLexer.GetCurrentLocation());
            return nullptr;
        }
    }
    
    // The expression has come to its natural end, return the expression node we built
    if (std::find(terminatorsBegin, terminatorsEnd, mCurrentToken) != terminatorsEnd)
    {
        return std::move(node);
    }

    // The expression has a trailing part and isn't well-formed, log an error
    ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WRONG_OPERATION, mLexer.GetCurrentLocation());
    return nullptr;
}

std::unique_ptr<Expr> Parser::ParseArrayExpr()
{
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();
    std::unique_ptr<Expr> aExpr;
    std::vector<std::unique_ptr<Expr>> arrElems;

    while ((mCurrentToken = mLexer.GetNextToken()) != Lexer::Token::RIGHT_BRACE)
    {
        arrElems.emplace_back(ParseExpr());

        // Array elements must be separated by a comma
        if (mCurrentToken != Lexer::Token::COMMA)
        {
            // TODO: Log an error and add a unit for it
            return aExpr;
        }
    }

    aExpr.reset(new ArrayExpr(std::move(arrElems), srcLoc));

    return aExpr;
}

std::unique_ptr<Expr> Parser::ParseBinaryOpExpr(Lexer::Token op, std::unique_ptr<Expr>&& lhs)
{
    // Parse the right-hand side of the expression. It is assumed that the left-hand side of the expression has been parsed.

    mCurrentToken = mLexer.GetNextToken();
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();

    std::unique_ptr<Expr> rhs = ParseExpr();
    if (rhs == nullptr)
    {
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::MISSING_RHS, mLexer.GetCurrentLocation());
        return nullptr;
    }
    else
    {
        return std::make_unique<BinaryOpExpr>(TokenToOpcode(op), std::move(lhs), std::move(rhs), srcLoc);
    }
}

std::unique_ptr<Expr> Parser::ParseCallExpr(std::unique_ptr<Expr>&& fn)
{
    std::unique_ptr<Expr> cExpr;
    std::vector<std::unique_ptr<Expr>> args;
    std::unique_ptr<Expr> arg;

    mCurrentToken = mLexer.GetNextToken();
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();

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

    cExpr.reset(new CallExpr(fn->GetName(), std::move(args), srcLoc));

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
        case Lexer::Token::COMMENT:
        case Lexer::Token::ML_COMMENT:
            mCurrentToken = mLexer.GetNextToken();
            continue;
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
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();

    std::unique_ptr<Expr> condExpr = ParseExpr();
    if (condExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::IF_MISSING_COND, mLexer.GetCurrentLocation());

    std::unique_ptr<CompoundStmt> thenStmt = ParseCompoundStmt();
    if (thenStmt == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::IF_MISSING_BODY, mLexer.GetCurrentLocation());
        
    if ((condExpr != nullptr) && (thenStmt != nullptr))
        ifStmt.reset(new IfStmt(std::move(condExpr), std::move(thenStmt), srcLoc));

    return ifStmt;
}

std::unique_ptr<PrintStmt> Parser::ParsePrintStmt()
{
    std::unique_ptr<PrintStmt> pStmt = std::make_unique<PrintStmt>(mLexer.GetCurrentLocation());

    mCurrentToken = mLexer.GetNextToken();
    if (mCurrentToken != Lexer::Token::SEMI_COLON)
    {
        std::unique_ptr<Expr> msgExpr = ParseExpr();

        if (msgExpr == nullptr)
        {
            ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::PRINT_WRONG_INPUT_TYPE, mLexer.GetCurrentLocation());
            pStmt->SetErrorMessage();
        }
        else
        {
            pStmt->AddMessage(std::move(msgExpr));
        }
    }

    return pStmt;
}

std::unique_ptr<ReturnStmt> Parser::ParseReturnStmt()
{
    auto rStmt = std::make_unique<ReturnStmt>(mLexer.GetCurrentLocation());

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
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();

    std::unique_ptr<Expr> inputExpr = ParseExpr();

    if (inputExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SCAN_MISSING_INPUT_VAR, mLexer.GetCurrentLocation());
    else if(inputExpr->GetKind() != ASTNode::NodeKind::IDENTIFIER_EXPR)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::SCAN_WRONG_INPUT_TYPE, mLexer.GetCurrentLocation());
    else
        sStmt.reset(new ScanStmt(std::make_unique<IdentifierExpr>(inputExpr->GetName(), mLexer.GetCurrentLocation()), srcLoc));
        
    return sStmt;
}

std::unique_ptr<WhileStmt> Parser::ParseWhileStmt()
{
    std::unique_ptr<WhileStmt> whileStmt = std::make_unique<WhileStmt>();

    mCurrentToken = mLexer.GetNextToken();
    const SourceLocation srcLoc = mLexer.GetCurrentLocation();

    std::unique_ptr<Expr> condExpr = ParseExpr();
    if (condExpr == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WHILE_MISSING_COND, mLexer.GetCurrentLocation());

    std::unique_ptr<CompoundStmt> body = ParseCompoundStmt();
    if (body == nullptr)
        ErrorLogger::PrintErrorAtLocation(ErrorLogger::ErrorType::WHILE_MISSING_BODY, mLexer.GetCurrentLocation());

    if ((condExpr != nullptr) && (body != nullptr))
        whileStmt.reset(new WhileStmt(std::move(condExpr), std::move(body), srcLoc));
    return whileStmt;
}

//////////////////// Static functions ////////////////////
static Operation TokenToOpcode(Lexer::Token tok)
{
    switch (tok)
    {
    case TosLang::FrontEnd::Lexer::Token::ASSIGN:
        return Operation::ASSIGNMENT;
    case TosLang::FrontEnd::Lexer::Token::AND_BOOL:
        return Operation::AND_BOOL;
    case TosLang::FrontEnd::Lexer::Token::AND_INT:
        return Operation::AND_INT;
    case TosLang::FrontEnd::Lexer::Token::DIVIDE:
        return Operation::DIVIDE;
    case TosLang::FrontEnd::Lexer::Token::EQUAL:
        return Operation::EQUAL;
    case TosLang::FrontEnd::Lexer::Token::GREATER_THAN:
        return Operation::GREATER_THAN;
    case TosLang::FrontEnd::Lexer::Token::LEFT_SHIFT:
        return Operation::LEFT_SHIFT;
    case TosLang::FrontEnd::Lexer::Token::LESS_THAN:
        return Operation::LESS_THAN;
    case TosLang::FrontEnd::Lexer::Token::MINUS:
        return Operation::MINUS;
    case TosLang::FrontEnd::Lexer::Token::MODULO:
        return Operation::MODULO;
    case TosLang::FrontEnd::Lexer::Token::MULT:
        return Operation::MULT;
    case TosLang::FrontEnd::Lexer::Token::NOT:
        return Operation::NOT;
    case TosLang::FrontEnd::Lexer::Token::OR_BOOL:
        return Operation::OR_BOOL;
    case TosLang::FrontEnd::Lexer::Token::OR_INT:
        return Operation::OR_INT;
    case TosLang::FrontEnd::Lexer::Token::PLUS:
        return Operation::PLUS;
    case TosLang::FrontEnd::Lexer::Token::RIGHT_SHIFT:
        return Operation::RIGHT_SHIFT;
    default:
        return Operation::UNKNOWN_OP;
    }
}
