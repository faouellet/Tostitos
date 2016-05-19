#include "interpreter.h"

#include "../AST/ast.h"

using namespace TosLang::FrontEnd;

void Interpreter::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab) { }

////////// Declarations //////////
void Interpreter::HandleFunctionDecl() { }
void Interpreter::HandleParamVarDecl() { }
void Interpreter::HandleProgramDecl() { }
void Interpreter::HandleVarDecl() { }

////////// Expressions //////////
void Interpreter::HandleBinaryExpr() { }
void Interpreter::HandleBooleanExpr() { }
void Interpreter::HandleCallExpr() { }
void Interpreter::HandleIdentifierExpr() { }
void Interpreter::HandleNumberExpr() { }
void Interpreter::HandleStringExpr() { }
     
////////// Statements //////////
void Interpreter::HandleCompoundStmt() { }
void Interpreter::HandleIfStmt() { }
void Interpreter::HandlePrintStmt() { }
void Interpreter::HandleReturnStmt() { }
void Interpreter::HandleScanStmt() { }
void Interpreter::HandleWhileStmt() { }