#ifndef EXPR_H__TOSTITOS
#define EXPR_H__TOSTITOS

#include "ast.h"

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
			explicit Expr(NodeKind kind) : ASTNode(kind) { }
            virtual ~Expr() { }
        };

		/*
		* \class BinaryOpExpr
		* \brief Node of the AST representing a binary expression be it arithmetic (+, -, *, \, %) or logical (||, &&)
		*/
		class BinaryOpExpr : public Expr
		{
		public:
			BinaryOpExpr(char op, std::unique_ptr<Expr>&& lhs, std::unique_ptr<Expr>&& rhs) : Expr(BINARY_EXPR), mOp{ op } 
			{
				mChildren.push_back(std::move(lhs));
				mChildren.push_back(std::move(rhs));
			}

			virtual ~BinaryOpExpr() { }
			
		private:
			char mOp;
		};

        /*
        * \class BooleanExpr
        * \brief Node of the AST representing a boolean literal
        */
        class BooleanExpr : public Expr
        {
        public:
			explicit BooleanExpr(bool value) : Expr(BOOLEAN_EXPR), mValue{ value } { mName = mValue ? "True" : "False"; }
            virtual ~BooleanExpr() { }

            const bool GetValue() const { return mValue; }

        private:
            bool mValue;
        };

        /*
        * \class IdentifierExpr 
        * \brief Node of the AST representing a string literal
        */
        class IdentifierExpr : public Expr
        {
        public:
			explicit IdentifierExpr(std::string value) : Expr(IDENTIFIER_EXPR) { mName = value; }
            virtual ~IdentifierExpr() { }
        };

        /*
        * \class NumberExpr
        * \brief Node of the AST representing a number literal
        */
        class NumberExpr : public Expr
        {
        public:
			explicit NumberExpr(int value) : Expr(NUMBER_EXPR), mValue{ value } { mName = std::to_string(value); }
            virtual ~NumberExpr() { }

            const int GetValue() const { return mValue; }

        private:
            int mValue;
        };
    }
}

#endif // EXPR_H__TOSTITOS
