#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"
#include "statements.h"

#include "../Common/type.h"

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
            explicit Decl() : ASTNode{ NodeKind::ERROR } { }
            explicit Decl(NodeKind kind) : ASTNode{ kind } { }
            virtual ~Decl() = default;
        };

        /*
        * \class ProgramDecl
        * \brief Node of the AST representing a whole program
        */
        class ProgramDecl : public Decl
        {
        public:
            ProgramDecl() : Decl{ NodeKind::PROGRAM_DECL } { }
            virtual ~ProgramDecl() = default;

        public:
            /*
            * \fn           AddProgramStmt
            * \brief        Adds a declaration to the program
            * \param stmt   Declaration to add to the program
            */
            void AddProgramDecl(std::unique_ptr<Decl>&& stmt) { AddChildNode(std::move(stmt)); }

            /*
            * \fn       GetProgramDecls
            * \brief    Gets the declarations (functions and variables) contained within the program
            * \return   Sequence of declarations
            */
            ChildrenNodes& GetProgramDecls() { return mChildren; }
        };

        /*
        * \class VarDecl
        * \brief Node of the AST representing a variable declaration.
        *        For example: var MyIntVar : Int
        *                     var MyIntArrayVar : Int[10]
        */
        class VarDecl : public Decl
        {
        public:
            VarDecl() : Decl{ NodeKind::ERROR }, mType{ Common::Type::ERROR }, mIsFunctionParameter{ false }, mVarSize{ 0 } { }
            VarDecl(const std::string& varName, Common::Type type, bool isFuncParam, int varSize, const Utils::SourceLocation& srcLoc)
                : Decl{ NodeKind::VAR_DECL }, mType{ type }, mIsFunctionParameter{ isFuncParam }, mVarSize{ varSize }
            {
                mName = varName; 
                mSrcLoc = srcLoc;
            }
            virtual ~VarDecl() = default;

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

            /*
            * \fn       GetVarType
            * \brief    Gets the type of the variable
            * \return   Type of the variable
            */
            const Common::Type GetVarType() const { return mType; }

            /*
            * \fn       IsFunctionParameter
            * \brief    Indicates if the variable is a function parameter
            * \return   True if the variable is a function parameter
            */
            const bool IsFunctionParameter() const { return mIsFunctionParameter; }

        private:
            Common::Type mType;         /*!< Variable type */
            bool mIsFunctionParameter;  /*!< Is the variable a function parameter */
            int mVarSize;               /*!< Number of elements contained in the variable. Only makes sense for an array */
        };

        /*
        * \class ParamVarDecl
        * \brief Node of the AST representing parameters to a function
        */
        class ParamVarDecls : public Decl
        {
        public:
            ParamVarDecls() : Decl{ NodeKind::PARAM_VAR_DECL } { }
            virtual ~ParamVarDecls() = default;

        public:
            /*
            * \fn           AddParameter
            * \brief        Add a parameter to the parameter list
            * \param param  Parameter to be added
            */
            void AddParameter(std::unique_ptr<VarDecl>&& param) { AddChildNode(std::move(param)); }

            /*
            * \fn           GetParameters
            * \brief        Gets the list of parameters
            * \return       List of parameters to a function
            */
            const ChildrenNodes& GetParameters() const { return mChildren; }
        };

        /*
        * \class FunctionDecl
        * \brief Node of the AST representing a function declaration with or without a definition.
        *        For example: fn MyFunc(): Int
        */
        class FunctionDecl : public Decl
        {
        public:
            FunctionDecl() : Decl{ NodeKind::ERROR }, mReturnType{ Common::Type::ERROR } { }
            FunctionDecl(const std::string& fnName, Common::Type type, 
                         std::unique_ptr<ParamVarDecls>&& params, std::unique_ptr<CompoundStmt>&& body,
                         const Utils::SourceLocation& srcLoc)
                : Decl{ NodeKind::FUNCTION_DECL }, mReturnType{ type }
            {
                mName = fnName;
                mSrcLoc = srcLoc;
                AddChildNode(std::move(params));
                AddChildNode(std::move(body));
            }

            virtual ~FunctionDecl() = default;

        public:
            /*
            * \fn       GetFunctionName
            * \brief    Gets the name of the function
            * \return   Name of the function
            */
            const std::string& GetFunctionName() const { return mName; }

            /*
            * \fn       GetReturnType
            * \brief    Gets the type of the value returned by the function
            * \return   Type of the value returned by the function
            */
            const Common::Type GetReturnType() const { return mReturnType; }

            /*
            * \fn       GetParametersDecl
            * \brief    Gets the parammeters declarations of the function
            * \return   Arguments of the function
            */
            const ParamVarDecls* GetParametersDecl() const { assert(mChildren.size() == 2); return GetChildNodeAs<ParamVarDecls>(0); }

            /*
            * \fn       GetParametersSize
            * \brief    Gets the number of parammeters declarations of the function
            * \return   Number of arguments of the function
            */
            size_t GetParametersSize() const { assert(mChildren.size() == 2); return GetChildNodeAs<ParamVarDecls>(0)->GetChildrenNodes().size(); }

            /*
            * \fn       GetBody
            * \brief    Gets the body of the function
            * \return   Body of the function
            */
            const CompoundStmt* GetBody() const { assert(mChildren.size() == 2); return GetChildNodeAs<CompoundStmt>(1); }

        private:
            Common::Type mReturnType; /*!< Function return type */
        };
    }
}

#endif // DECL_H__TOSTITOS
