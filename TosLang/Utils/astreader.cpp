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
    mNodeKindRegexes[ASTNode::NodeKind::VAR_DECL] = std::regex("VarDecl: ([a-zA-Z].*) Type: ([a-zA-Z].*)");
    mNodeKindRegexes[ASTNode::NodeKind::PROGRAM_DECL] = std::regex("ProgramDecl");
    
    // Expressions
    mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR] = std::regex("BooleanExpr: (True|False)");
    mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR] = std::regex("NumberExpr: ([1-9][0-9]*)");
    
    // Statements
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
    if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::BOOLEAN_EXPR]))
        return std::make_unique<BooleanExpr>(mCurrentMatch[0].str() == "True");
    else if (std::regex_match(mCurrentLine, mCurrentMatch, mNodeKindRegexes[ASTNode::NodeKind::NUMBER_EXPR]))
        return std::make_unique<NumberExpr>(std::stoi(mCurrentMatch[0].str()));
    else
        return std::make_unique<Expr>(ASTNode::NodeKind::ERROR);
}
