#include "astreader.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../AST/statements.h"

#include <fstream>

using namespace TosLang::FrontEnd;
using namespace TosLang::Utils;

ASTReader::ASTReader() : mStream{}, mCurrentLine{}
{
    // TODO: Should be a constexpr member variable

    // Declaration
    mNodeKindRegexes[ASTNode::NodeKind::VAR_DECL] = std::regex("VarDecl: ([a-zA-Z].*) Type: ([0-9]+)");
    mNodeKindRegexes[ASTNode::NodeKind::FUNCTION_DECL] = std::regex("FunctionDecl: ([a-zA-Z].*) Return Type: ([0-9]+)");
    mNodeKindRegexes[ASTNode::NodeKind::PROGRAM_DECL] = std::regex("ProgramDecl");
    
    // Expressions
    mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR] = std::regex("BinaryExpr: ([0-9]+)");
    mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR] = std::regex("BooleanExpr: (True|False)");
    mNodeKindRegexes[ASTNode::NodeKind::IDENTIFIER_EXPR] = std::regex("IdentifierExpr: (.*)");
    mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR] = std::regex("NumberExpr: ([1-9][0-9]*)");
    mNodeKindRegexes[ASTNode::NodeKind::STRING_EXPR] = std::regex("StringExpr: (.*)");
    
    // Statements
    mNodeKindRegexes[ASTNode::NodeKind::COMPOUND_STMT] = std::regex("CompoundStmt:");
    mNodeKindRegexes[ASTNode::NodeKind::IF_STMT] = std::regex("IfStmt:");
    mNodeKindRegexes[ASTNode::NodeKind::PRINT_STMT] = std::regex("PrintStmt:");
    mNodeKindRegexes[ASTNode::NodeKind::RETURN_STMT] = std::regex("ReturnStmt:");
    mNodeKindRegexes[ASTNode::NodeKind::SCAN_STMT] = std::regex("ScanStmt:");
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

//////////////////// Declarations ////////////////////

std::unique_ptr<FunctionDecl> ASTReader::ReadFuncDecl()
{
    return std::unique_ptr<FunctionDecl>();
}

std::unique_ptr<VarDecl> ASTReader::ReadVarDecl()
{
    // Get the name of the variable
    assert(mCurrentMatch.size() == 2);
    
    // Create the VarDecl node
    auto vDecl = std::make_unique<VarDecl>(mCurrentMatch[0].str(), static_cast<Common::Type>(std::stoi(mCurrentMatch[0].str())), /*isFuncParam=*/false);

    // Add an initialization expression if needed
    size_t oldIndent = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    std::getline(mStream, mCurrentLine);
    size_t newIndent = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
    if (oldIndent != newIndent)
    {
        vDecl->AddInitialization(ReadExpr());
    }
    
    return vDecl;
}

//////////////////// Expressions ////////////////////

std::unique_ptr<Expr> ASTReader::ReadExpr()
{
    if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR]))
    {
        std::smatch binOpMatch = mCurrentMatch;
        std::getline(mStream, mCurrentLine);
        auto lhs = ReadExpr();
        std::getline(mStream, mCurrentLine);
        auto rhs = ReadExpr();
        std::getline(mStream, mCurrentLine);
        return std::make_unique<BinaryOpExpr>(static_cast<Common::Opcode>(std::stoi(binOpMatch[0].str())), std::move(lhs), std::move(rhs));

    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR]))
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<BooleanExpr>(mCurrentMatch[0].str() == "True");
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::CALL_EXPR]))
    {
        const size_t callIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        
        std::getline(mStream, mCurrentLine);
        size_t currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        
        std::vector<std::unique_ptr<Expr>> args;
        while (currentIndentLevel > callIndentLevel)
        {
            args.emplace_back(ReadExpr());
            std::getline(mStream, mCurrentLine);
            currentIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        }

        return std::make_unique<CallExpr>(mCurrentMatch[0].str(), std::move(args));
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::IDENTIFIER_EXPR]))
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<IdentifierExpr>(mCurrentMatch[0].str());
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR]))
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<NumberExpr>(std::stoi(mCurrentMatch[0].str()));
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::STRING_EXPR]))
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<StringExpr>(mCurrentMatch[0].str());
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

    while (currentIndentLevel < topIndentLevel)
    {
        if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::FUNCTION_DECL]))
            cStmt->AddStatement(ReadVarDecl());
        else if (std::any_of(std::begin(stmtKinds), std::end(stmtKinds), [this](ASTNode::NodeKind kind) { return std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[kind]); }))
            cStmt->AddStatement(ReadStmt());
        else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BINARY_EXPR]) 
                || std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::CALL_EXPR]))
            cStmt->AddStatement(ReadExpr());
    }

    return std::move(cStmt);
}

std::unique_ptr<Stmt> ASTReader::ReadStmt()
{
    if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::COMPOUND_STMT]))
    {
        return ReadCompoundStmt();
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::IF_STMT]))
    {
        std::getline(mStream, mCurrentLine);
        auto condExpr = ReadExpr();
        auto body = ReadCompoundStmt();
        return std::make_unique<IfStmt>(std::move(condExpr), std::move(body));
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::PRINT_STMT]))
    {
        auto pStmt = std::make_unique<PrintStmt>();

        const size_t returnIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');
        std::getline(mStream, mCurrentLine);
        const size_t nextIndentLevel = std::count(mCurrentLine.begin(), mCurrentLine.end(), '\t');

        if (returnIndentLevel < nextIndentLevel)
            pStmt->AddMessage(ReadExpr());

        return std::move(pStmt);
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::RETURN_STMT]))
    {
        auto rStmt = std::make_unique<ReturnStmt>();

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
        return std::make_unique<ScanStmt>(std::make_unique<IdentifierExpr>(expr->GetName()));
    }
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::WHILE_STMT]))
    {
        std::getline(mStream, mCurrentLine);
        auto condExpr = ReadExpr();
        auto body = ReadCompoundStmt();
        return std::make_unique<WhileStmt>(std::move(condExpr), std::move(body));
    }
    else
    {
        std::getline(mStream, mCurrentLine);
        return std::make_unique<Stmt>(ASTNode::NodeKind::ERROR);
    }
}
