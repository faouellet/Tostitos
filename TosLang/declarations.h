#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"

#include <memory>
#include <string>
#include <vector>

typedef std::vector<std::unique_ptr<ASTNode>> ChildrenNodes;

/*
* \class ProgramDecl
* \brief Node of the AST representing a whole program
*/
class ProgramDecl : public ASTNode
{
public:
    ProgramDecl() { }
    virtual ~ProgramDecl() { }

    void AddChildNode(std::unique_ptr<ASTNode>&& node) { mChildren.push_back(std::move(node)); }
    const ChildrenNodes& GetChildrenNodes() const { return mChildren; }

private:
    ChildrenNodes mChildren;
};

/*
* \class VarDecl
* \brief Node of the AST representing a variable declaration.
*        For example: var MyIntVar: Int
*/
class VarDecl : public ASTNode
{
public:
    VarDecl(const std::string& varName) : mVarName(varName), mInitExpr{ nullptr } { }
    virtual ~VarDecl() { }

    void AddValue(std::unique_ptr<Expr>&& value) { mInitExpr = std::move(value); }

    const std::unique_ptr<Expr>& GetInitExpr() const { return mInitExpr; }
    const std::string& GetVarName() const { return mVarName; }

private:
    std::string mVarName;
    std::unique_ptr<Expr> mInitExpr;
};

#endif // DECL_H__TOSTITOS
