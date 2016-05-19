#ifndef INTERPRETER_H__TOSTITOS
#define INTERPRETER_H__TOSTITOS

#include "../Common/astvisitor.h"

#include <memory>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class SymbolTable;
    }
}

// TODO: In a namespace?

class Interpreter : public TosLang::Common::ASTVisitor<Interpreter>
{
    friend class TosLang::Common::ASTVisitor<Interpreter>;
    
public:
    void Run(const std::unique_ptr<TosLang::FrontEnd::ASTNode>& root, const std::shared_ptr<TosLang::FrontEnd::SymbolTable>& symTab);

protected:  // Declarations
    void HandleFunctionDecl();
    void HandleParamVarDecl();
    void HandleProgramDecl();
    void HandleVarDecl();

protected:  // Expressions
    void HandleBinaryExpr();
    void HandleBooleanExpr();
    void HandleCallExpr();
    void HandleIdentifierExpr();
    void HandleNumberExpr();
    void HandleStringExpr();

protected:  // Statements
    void HandleCompoundStmt();
    void HandleIfStmt();
    void HandlePrintStmt();
    void HandleReturnStmt();
    void HandleScanStmt();
    void HandleWhileStmt();

private:

};

#endif // INTERPRETER_H__TOSTITOS
