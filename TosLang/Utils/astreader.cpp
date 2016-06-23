#include "astreader.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../AST/statements.h"
#include "sourceloc.h"

#include <fstream>

#include <iostream>

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

// TODO: Modify the AST printer and the AST reader to handle array variables

ASTReader::ASTReader() : mStream{}, mCurrentLine{}, mSrcLocRegex{ ".*SrcLoc: ([1-9][0-9]*), ([1-9][0-9]*)" }
{
    // TODO: Should be a constexpr member variable

    // Declaration
    mNodeKindRegexes[ASTNode::NodeKind::FUNCTION_DECL]      = std::regex("\t+FunctionDecl: ([a-zA-Z][^ ]*) Return Type: ([0-9]+).*");
    mNodeKindRegexes[ASTNode::NodeKind::PARAM_VAR_DECL]     = std::regex("\t+ParamVarDecl: ([a-zA-Z].*) Type: ([0-9]+).*");
    mNodeKindRegexes[ASTNode::NodeKind::PROGRAM_DECL]       = std::regex("ProgramDecl");
    mNodeKindRegexes[ASTNode::NodeKind::VAR_DECL]           = std::regex("\t+VarDecl: ([a-zA-Z].*) Type: ([0-9]+).*");
                                                                            
    // Expressions                                                          
    mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR]        = std::regex("\t+BinaryOpExpr: ([0-9]+).*");
    mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR]       = std::regex("\t+BooleanExpr: (True|False).*");
    mNodeKindRegexes[ASTNode::NodeKind::CALL_EXPR]          = std::regex("\t+CallExpr: ([a-zA-Z][^ ]*).*");
    mNodeKindRegexes[ASTNode::NodeKind::IDENTIFIER_EXPR]    = std::regex("\t+IdentifierExpr: ([a-zA-Z][^ ]*).*");
    mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR]        = std::regex("\t+NumberExpr: ([1-9][0-9]*).*");
    mNodeKindRegexes[ASTNode::NodeKind::STRING_EXPR]        = std::regex("\t+StringExpr: \"([^\"]*).*");
                                                                            
    // Statements                                                           
    mNodeKindRegexes[ASTNode::NodeKind::COMPOUND_STMT]      = std::regex("\t+CompoundStmt");
    mNodeKindRegexes[ASTNode::NodeKind::IF_STMT]            = std::regex("\t+IfStmt.*");
    mNodeKindRegexes[ASTNode::NodeKind::PRINT_STMT]         = std::regex("\t+PrintStmt.*");
    mNodeKindRegexes[ASTNode::NodeKind::RETURN_STMT]        = std::regex("\t+ReturnStmt.*");
    mNodeKindRegexes[ASTNode::NodeKind::SCAN_STMT]          = std::regex("\t+ScanStmt.*");
    mNodeKindRegexes[ASTNode::NodeKind::WHILE_STMT]         = std::regex("\t+WhileStmt.*");
}

std::unique_ptr<ASTNode> ASTReader::Run(const std::string& filename)
{
    mStream.open(filename);
    if (!mStream.is_open())
        return nullptr;

    std::getline(mStream, mCurrentLine);

    // AST file must start with a ProgramDecl node
    if (!std::regex_match(mCurrentLine, mNodeKindRegexes[ASTNode::NodeKind::PROGRAM_DECL]))
    {
        mStream.close();
        return nullptr;
    }

    auto astRoot = std::make_unique<ProgramDecl>();
    std::getline(mStream, mCurrentLine);
    
    while (!mStream.eof())
    {
        if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::FUNCTION_DECL]))
            astRoot->AddProgramDecl(ReadFuncDecl());
        else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::VAR_DECL]))
            astRoot->AddProgramDecl(ReadVarDecl());
        else
            mStream.close();
    }

    return std::move(astRoot);
}

const SourceLocation ASTReader::ReadSourceLocation()
{
    std::smatch match;
    if (std::regex_match(mCurrentLine, match, mSrcLocRegex))
        return SourceLocation(std::stoi(match[1].str()), std::stoi(match[2].str()));
    
    return SourceLocation{};
}

//////////////////// Declarations ////////////////////

std::unique_ptr<FunctionDecl> ASTReader::ReadFuncDecl()
{
    assert(mCurrentMatch.size() == 3);

    // Get the function declaration information
    SourceLocation srcLoc = ReadSourceLocation();
    const std::string fnName = mCurrentMatch[1].str();
    const Common::Type fnType = static_cast<Common::Type>(std::stoi(mCurrentMatch[2].str()));

    // Read the parameters
    auto params = std::make_unique<ParamVarDecls>();
    std::getline(mStream, mCurrentLine);
    while(std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::PARAM_VAR_DECL]))
        params->AddParameter(ReadVarDecl(/*isFuncParam=*/true));
    
    // Read the body
    std::unique_ptr<CompoundStmt> body = ReadCompoundStmt();

    // Create the FunctionDecl node
    return std::make_unique<FunctionDecl>(fnName, fnType, std::move(params), std::move(body), srcLoc);
}

std::unique_ptr<VarDecl> ASTReader::ReadVarDecl(const bool isFuncParam)
{
    assert(mCurrentMatch.size() == 3);
    
    // Get the source location information
    SourceLocation srcLoc = ReadSourceLocation();

    // Create the VarDecl node
    auto vDecl = std::make_unique<VarDecl>(mCurrentMatch[1].str(), 
                                           static_cast<Common::Type>(std::stoi(mCurrentMatch[2].str())), 
                                           /*isFuncParam=*/isFuncParam, 0, srcLoc);

    // Add an initialization expression if needed
    size_t oldIndent = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    std::getline(mStream, mCurrentLine);
    size_t newIndent = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    if (oldIndent < newIndent)
        vDecl->AddInitialization(ReadExpr());
    
    return vDecl;
}

//////////////////// Expressions ////////////////////

std::unique_ptr<Expr> ASTReader::ReadExpr()
{
    // Get the source location information
    SourceLocation srcLoc = ReadSourceLocation();

    if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR]))
    {
        const std::string binOpStr = mCurrentMatch[1].str();
    
        std::getline(mStream, mCurrentLine);
        
        auto lhs = ReadExpr();
        auto rhs = ReadExpr();

        return std::make_unique<BinaryOpExpr>(static_cast<Common::Operation>(std::stoi(binOpStr)), std::move(lhs), std::move(rhs), srcLoc);

    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR]))
    {      
        const std::string boolStr = mCurrentMatch[1].str();
        std::getline(mStream, mCurrentLine);
        return std::make_unique<BooleanExpr>(boolStr == "True", srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::CALL_EXPR]))
    {
        const std::string callStr = mCurrentMatch[1].str();

        const size_t callIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        
        std::getline(mStream, mCurrentLine);
        size_t currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        
        std::vector<std::unique_ptr<Expr>> args;
        while (currentIndentLevel > callIndentLevel)
        {
            args.emplace_back(ReadExpr());
            currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        }

        //std::getline(mStream, mCurrentLine);
        return std::make_unique<CallExpr>(callStr, std::move(args), srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::IDENTIFIER_EXPR]))
    {
        const std::string identifierStr = mCurrentMatch[1].str();
        std::getline(mStream, mCurrentLine);
        return std::make_unique<IdentifierExpr>(identifierStr, srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR]))
    {
        const std::string numberStr = mCurrentMatch[1].str();
        std::getline(mStream, mCurrentLine);
        return std::make_unique<NumberExpr>(std::stoi(numberStr), srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::STRING_EXPR]))
    {
        const std::string str = mCurrentMatch[1].str();
        std::getline(mStream, mCurrentLine);
        return std::make_unique<StringExpr>(str, srcLoc);
    }
    else
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<Expr>(ASTNode::NodeKind::ERROR);
    }
}

//////////////////// Statements ////////////////////

std::unique_ptr<CompoundStmt> ASTReader::ReadCompoundStmt()
{
    auto cStmt = std::make_unique<CompoundStmt>();

    const size_t topIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    std::getline(mStream, mCurrentLine);
    size_t currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');

    ASTNode::NodeKind stmtKinds[] = { 
        ASTNode::NodeKind::COMPOUND_STMT, ASTNode::NodeKind::IF_STMT, ASTNode::NodeKind::PRINT_STMT, 
        ASTNode::NodeKind::RETURN_STMT, ASTNode::NodeKind::SCAN_STMT, ASTNode::NodeKind::WHILE_STMT 
    };

    while (topIndentLevel < currentIndentLevel)
    {
        if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::VAR_DECL]))
            cStmt->AddStatement(ReadVarDecl());
        else if (std::any_of(std::begin(stmtKinds), std::end(stmtKinds), [this](ASTNode::NodeKind kind) { return std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[kind]); }))
            cStmt->AddStatement(ReadStmt());
        else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR]) 
                || std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::CALL_EXPR]))
            cStmt->AddStatement(ReadExpr());

        currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    }

    return std::move(cStmt);
}

std::unique_ptr<Stmt> ASTReader::ReadStmt()
{
    // Get the source location information
    SourceLocation srcLoc = ReadSourceLocation();

    if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::COMPOUND_STMT]))
    {
        return ReadCompoundStmt();
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::IF_STMT]))
    {
        std::getline(mStream, mCurrentLine);
        auto condExpr = ReadExpr();
        auto body = ReadCompoundStmt();
        return std::make_unique<IfStmt>(std::move(condExpr), std::move(body), srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::PRINT_STMT]))
    {
        auto pStmt = std::make_unique<PrintStmt>(srcLoc);

        const size_t returnIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        std::getline(mStream, mCurrentLine);
        const size_t nextIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');

        if (returnIndentLevel < nextIndentLevel)
            pStmt->AddMessage(ReadExpr());

        return std::move(pStmt);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::RETURN_STMT]))
    {
        auto rStmt = std::make_unique<ReturnStmt>(srcLoc);

        const size_t returnIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        std::getline(mStream, mCurrentLine);
        const size_t nextIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');

        if (returnIndentLevel < nextIndentLevel)
            rStmt->AddReturnValue(ReadExpr());

        return std::move(rStmt);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::SCAN_STMT]))
    {
        auto expr = ReadExpr();
        assert(expr->GetKind() == ASTNode::NodeKind::IDENTIFIER_EXPR);
        return std::make_unique<ScanStmt>(std::make_unique<IdentifierExpr>(expr->GetName(), expr->GetSourceLocation()), srcLoc);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::WHILE_STMT]))
    {
        std::getline(mStream, mCurrentLine);
        auto condExpr = ReadExpr();
        auto body = ReadCompoundStmt();
        return std::make_unique<WhileStmt>(std::move(condExpr), std::move(body), srcLoc);
    }
    else
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<Stmt>(ASTNode::NodeKind::ERROR);
    }
}
