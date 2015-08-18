#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"

#include <cassert>
#include <string>

/*
* \class ProgramDecl
* \brief Node of the AST representing a whole program
*/
class ProgramDecl : public ASTNode
{
public:
    ProgramDecl() : ASTNode(PROGRAM_DECL) { }
    virtual ~ProgramDecl() { }
};

/*
* \class VarDecl
* \brief Node of the AST representing a variable declaration.
*        For example: var MyIntVar: Int
*/
class VarDecl : public ASTNode
{
public:
    VarDecl(const std::string& varName) : ASTNode(VAR_DECL), mVarName(varName) { }
    virtual ~VarDecl() { }

    void AddValue(std::unique_ptr<Expr>&& value) { mChildren.push_back(std::move(value)); }

    const std::unique_ptr<ASTNode>& GetInitExpr() const { assert(mChildren.size() < 2); return mChildren[0]; }
    const std::string& GetVarName() const { return mVarName; }

private:
    std::string mVarName;
};

#endif // DECL_H__TOSTITOS
