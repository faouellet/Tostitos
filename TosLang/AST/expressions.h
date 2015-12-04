#ifndef EXPR_H__TOSTITOS
#define EXPR_H__TOSTITOS

#include "ast.h"
#include "../Common/opcodes.h"

namespace TosLang
{
    namespace FrontEnd
    {
        /*
        * \class Expr
        * \brief Node of the AST representing a value or an expression producing a value
        */
        class Expr : public ASTNode
        {
        public:
            explicit Expr(NodeKind kind) : ASTNode{ kind } { }
            virtual ~Expr() { }
        };

		/*
		* \class BinaryOpExpr
		* \brief Node of the AST representing a binary expression be it arithmetic (+, -, *, \, %) or logical (||, &&)
		*/
		class BinaryOpExpr : public Expr
		{
		public:
            BinaryOpExpr(Common::Opcode op, std::unique_ptr<Expr>&& lhs, std::unique_ptr<Expr>&& rhs) : 
                Expr{ NodeKind::BINARY_EXPR }, mOp{ op }
            {
                assert(lhs != nullptr);
                assert(rhs != nullptr);

                AddChildNode(std::move(lhs));
                AddChildNode(std::move(rhs));
            }

			virtual ~BinaryOpExpr() { }

        public:
            /*
            * \fn       GetLHS
            * \brief    Gets the left hand side of the binary expression
            * \return   Expression on the left side of the operation
            */
            const Expr* GetLHS() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(0); }

            /*
            * \fn       GetOperation
            * \brief    Gets the operation applied in the binary expression
            * \return   Operation to be applied in the binary expression
            */
            const Common::Opcode GetOperation() const { return mOp; }

            /*
            * \fn       GetRHS
            * \brief    Gets the right hand side of the binary expression
            * \return   Expression on the right side of the operation
            */
            const Expr* GetRHS() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(1); }

		private:
			Common::Opcode mOp;     /*!< Operation applied in the binary expression */
		};

        /*
        * \class BooleanExpr
        * \brief Node of the AST representing a boolean literal
        */
        class BooleanExpr : public Expr
        {
        public:
            explicit BooleanExpr(bool value) : Expr{ NodeKind::BOOLEAN_EXPR }, mValue{ value } { mName = mValue ? "True" : "False"; }
            virtual ~BooleanExpr() { }

        public:
            /*
            * \fn       GetValue
            * \brief    Gets the value associated with the BooleanExpr AST node
            * \return   Boolean value of the AST node
            */
            const bool GetValue() const { return mValue; }

        private:
            bool mValue;    /*!< Boolean value of the boolean literal */
        };

        /*
        * \class CallExpr
        * \brief Node of the AST representing a function call
        */
        class CallExpr : public Expr
        {
        public:
            CallExpr(const std::string& fnName, std::vector<std::unique_ptr<Expr>>&& args) : Expr{ NodeKind::CALL_EXPR }
            { 
                mName = fnName; 
                mChildren.insert(mChildren.end(), std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
            }
            
            virtual ~CallExpr() { }

        public:
            /*
            * \fn       GetCalleeName
            * \brief    Gets the name of the function being called
            * \return   Name of the callee
            */
            const std::string& GetCalleeName() const { return mName; }

            /*
            * \fn       GetArgs
            * \brief    Gets the arguments used to call a function
            * \return   List of argument expressions
            */
            const std::vector<std::unique_ptr<ASTNode>>& GetArgs() const { return mChildren; }
        };

        /*
        * \class IdentifierExpr 
        * \brief Node of the AST representing a string literal
        */
        class IdentifierExpr : public Expr
        {
        public:
            explicit IdentifierExpr(std::string value) : Expr{ NodeKind::IDENTIFIER_EXPR } { mName = value; }
            virtual ~IdentifierExpr() { }
        };

        /*
        * \class NumberExpr
        * \brief Node of the AST representing a number literal
        */
        class NumberExpr : public Expr
        {
        public:
            explicit NumberExpr(int value) : Expr{ NodeKind::NUMBER_EXPR }, mValue{ value } { mName = std::to_string(value); }
            virtual ~NumberExpr() { }

        public:
            /*
            * \fn       GetValue
            * \brief    Gets the value associated with the NumberExpr AST node
            * \return   Integer value of the AST node
            */
            const int GetValue() const { return mValue; }

        private:
            int mValue;     /*!< Integer value of the number literal */
        };
    }
}

#endif // EXPR_H__TOSTITOS
