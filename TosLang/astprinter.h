#ifndef AST_PRINTER_H__TOSTITOS
#define AST_PRINTER_H__TOSTITOS

#include "astvisitor.h"

#include "declarations.h"
#include "expressions.h"

/*
* \class
* \brief Prints the AST the output stream specified as the template parameter
*/
template <typename OS>
class ASTPrinter : public ASTVisitor<ASTPrinter<OS>>
{
    friend class ASTVisitor<ASTPrinter<OS>>;

public:
    ASTPrinter(OS&& stream) : mStream(std::move(stream)) { }
    ~ASTPrinter() = default;

public:
    void Print(const std::unique_ptr<ASTNode>& root)
    {
        this->VisitPreOrder(root);
    }

protected:
    // Declarations
    void HandleProgramDecl() 
    {
        Indent();
        mStream << "ProgramDecl\n";
    }
    
    void HandleVarDecl() 
    {
        const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
        Indent();
        mStream << "VarDecl: " << vDecl->GetVarName() << "\n";
    }

    // Expressions
    void HandleBooleanExpr() 
    {
        const BooleanExpr* bExpr = dynamic_cast<const BooleanExpr*>(this->mCurrentNode);
        Indent();
        mStream << "BooleanExpr: ";

        if (bExpr->GetValue())
            mStream << "True";
        else
            mStream << "False";
        
        mStream<< "\n";
    }

    void HandleNumberExpr()
    {
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(this->mCurrentNode);
        Indent();
        mStream << "NumberExpr: " << nExpr->GetValue() << "\n";
    }

private:
    void Indent()
    {
        for (unsigned i = 0; i < this->mCurrentLevel; ++i)
            mStream << "\t";
    }

private:
    OS mStream;
};

#endif // AST_PRINTER_H__TOSTITOS
