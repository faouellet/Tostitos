#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"

#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class ProgramDecl
        * \brief Node of the AST representing a whole program
        */
        class ProgramDecl : public ASTNode
        {
        public:
            ProgramDecl() : ASTNode(NodeKind::PROGRAM_DECL) { }
            virtual ~ProgramDecl() { }

        public:
            /*
            * \fn           AddProgramStmt
            * \brief        Adds a statement to the program
            * \param stmt   Statement to add to the program
            */
            void AddProgramStmt(std::unique_ptr<ASTNode>&& stmt) { AddChildNode(std::move(stmt)); }

            /*
            * \fn       GetInitExpr
            * \brief    Gets the initialization expression linked to the variable declaration
            * \return   Pointer to the initialization expression
            */
            const ChildrenNodes& GetProgramStmts() const { return mChildren; }
        };

        /*
        * \class VarDecl
        * \brief Node of the AST representing a variable declaration.
        *        For example: var MyIntVar: Int
        */
        class VarDecl : public ASTNode
        {
        public:
            explicit VarDecl(const std::string& varName) : ASTNode(NodeKind::VAR_DECL) { mName = varName; }
            virtual ~VarDecl() { }

		public:
            /*
            * \fn           AddInitialization
            * \brief        Links an initialization expression to a variable declaration
            * \param value  Initialization expression for the variable
            * \return       true if the initialization expression could be added, false otherwise
            */
            bool AddInitialization(std::unique_ptr<Expr>&& value) 
            {
                if (value == nullptr)
                {
                    return false;
                }
                else
                {
                    AddChildNode(std::move(value));
                    return true;
                }
            }

            /*
            * \fn       GetInitExpr
            * \brief    Gets the initialization expression linked to the variable declaration
            * \return   Pointer to the initialization expression
            */
            const Expr* GetInitExpr() const { return mChildren.size() > 0 ? dynamic_cast<Expr*>(mChildren[0].get()) : nullptr; }

            /*
            * \fn       GetVarName
            * \brief    Gets the name of the variable
            * \return   Name of the variable
            */
            const std::string& GetVarName() const { return mName; }
        };
    }
}

#endif // DECL_H__TOSTITOS
