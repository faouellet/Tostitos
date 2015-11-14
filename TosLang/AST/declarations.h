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
            VarDecl() : Decl{ NodeKind::ERROR } { }
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
        class ParamVarDecls : public Decl
        {
        public:
            ParamVarDecls() : Decl{ NodeKind::PARAM_VAR_DECL } { }

        public:
            void AddParameter(std::unique_ptr<VarDecl>&& param) { AddChildNode(std::move(param)); }
            const ChildrenNodes& GetParameters() const { return mChildren; }

            virtual ~ParamVarDecls() { }
        };

        /*
        * \class FunctionDecl
        * \brief Node of the AST representing a function declaration with or without a definition.
        *        For example: fn MyFunc(): Int
        */
        class FunctionDecl : public Decl
        {
        public:
            FunctionDecl() : Decl{ NodeKind::ERROR } { }
            FunctionDecl(const std::string& fnName, std::unique_ptr<ParamVarDecls>&& params, std::unique_ptr<CompoundStmt>&& body) : Decl{ NodeKind::FUNCTION_DECL }
            {
                mName = fnName;
                AddChildNode(std::move(params));
                AddChildNode(std::move(body));
            }

            virtual ~FunctionDecl() { }

        public:
            /*
            * \fn       GetFunctionName
            * \brief    Gets the name of the function
            * \return   Name of the function
            */
            const std::string& GetFunctionName() const { return mName; }

            /*
            * \fn       GetArguments
            * \brief    Gets the arguments of the function
            * \return   Arguments of the function
            */
            const ParamVarDecls* GetArguments() const { assert(mChildren.size() == 2); return GetChildNodeAs<ParamVarDecls>(0); }

            /*
            * \fn       GetBody
            * \brief    Gets the body of the function
            * \return   Body of the function
            */
            const CompoundStmt* GetBody() const { assert(mChildren.size() == 2); return GetChildNodeAs<CompoundStmt>(1);; }
        };
    }
}

#endif // DECL_H__TOSTITOS
