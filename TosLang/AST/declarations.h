#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"
#include "statements.h"

#include <string>

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class Decl
        * \brief Node of the AST representing a declaration
        */
        class Decl : public ASTNode
        {
        public:
            explicit Decl(NodeKind kind) : ASTNode{ kind } { }
            virtual ~Decl() { }
        };

        /*
        * \class ProgramDecl
        * \brief Node of the AST representing a whole program
        */
        class ProgramDecl : public Decl
        {
        public:
            ProgramDecl() : Decl{ NodeKind::PROGRAM_DECL } { }
            virtual ~ProgramDecl() { }

        public:
            /*
            * \fn           AddProgramStmt
            * \brief        Adds a statement to the program
            * \param stmt   Statement to add to the program
            */
            void AddProgramStmt(std::unique_ptr<Decl>&& stmt) { AddChildNode(std::move(stmt)); }

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
        class VarDecl : public Decl
        {
        public:
            explicit VarDecl(const std::string& varName) : Decl{ NodeKind::VAR_DECL } { mName = varName; }
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
            const Expr* GetInitExpr() const { return mChildren.size() > 0 ? GetChildNodeAs<Expr>(0) : nullptr; }

            /*
            * \fn       GetVarName
            * \brief    Gets the name of the variable
            * \return   Name of the variable
            */
            const std::string& GetVarName() const { return mName; }
        };

        /*
        * \class ParamVarDecl
        * \brief Node of the AST representing parameters to a function
        */
        class ParamVarDecl : public Decl
        {
        public:
            ParamVarDecl() : Decl{ NodeKind::PARAM_VAR_DECL } { }

        public:
            void AddParameter(std::unique_ptr<VarDecl>&& param) { AddChildNode(std::move(param)); }

            virtual ~ParamVarDecl() { }
        };

        /*
        * \class FunctionDecl
        * \brief Node of the AST representing a function declaration with or without a definition.
        *        For example: fn MyFunc(): Int
        */
        class FunctionDecl : public Decl
        {
        public:
            FunctionDecl(const std::string& fnName, std::unique_ptr<ParamVarDecl>&& params) : Decl{ NodeKind::FUNCTION_DECL }
            {
                mName = fnName;
                AddChildNode(std::move(params));
            }

            virtual ~FunctionDecl() { }

        public:
            /*
            * \fn       GetFunctionName
            * \brief    Gets the name of the function
            * \return   Name of the function
            */
            const std::string& GetFunctionName() const { return mName; }
        };
    }
}

#endif // DECL_H__TOSTITOS
