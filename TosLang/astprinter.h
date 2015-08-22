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
    /*
    * \fn           Print
    * \param root   Root of the tree to print
    * \brief        Print the tree rooted at root.
    */
    void Print(const std::unique_ptr<ASTNode>& root)
    {
        this->VisitPreOrder(root);
    }

protected:  // Declarations
    /*
    * \fn       HandleNumberExpr
    * \brief    Prints a node of the PROGRAM_DECL kind
    */
    void HandleProgramDecl() 
    {
        Indent();
        mStream << "ProgramDecl\n";
    }

    /*
    * \fn       HandleNumberExpr
    * \brief    Prints a node of the VAR_DECL kind
    */
    void HandleVarDecl()
    {
        const VarDecl* vDecl = dynamic_cast<const VarDecl*>(this->mCurrentNode);
        Indent();
        mStream << "VarDecl: " << vDecl->GetVarName() << "\n";
    }

protected:  // Expressions
    /*
    * \fn       HandleNumberExpr
    * \brief    Prints a node of the BOOLEAN_EXPR kind
    */
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

    /*
    * \fn       HandleNumberExpr
    * \brief    Prints a node of the NUMBER_EXPR kind
    */
    void HandleNumberExpr()
    {
        const NumberExpr* nExpr = dynamic_cast<const NumberExpr*>(this->mCurrentNode);
        Indent();
        mStream << "NumberExpr: " << nExpr->GetValue() << "\n";
    }

private:
    /*
    * \fn       Indent
    * \brief    Indent the text to print according to the current level in the traversal
    */
    void Indent()
    {
        for (unsigned i = 0; i < this->mCurrentLevel; ++i)
            mStream << "\t";
    }

private:
    OS mStream;     /*!< Output stream to which the AST will be printed */
};

#endif // AST_PRINTER_H__TOSTITOS
