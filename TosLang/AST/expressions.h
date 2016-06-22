#ifndef EXPR_H__TOSTITOS
#define EXPR_H__TOSTITOS

#include "ast.h"
#include "../Common/opcodes.h"
#include "../Common/type.h"

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
            virtual ~Expr() = default;
            virtual bool IsLiteral() const { return false; }
        };

        /*
        * \class ArrayExpr
        * \brief Node of the AST representing an array expression.
        *        Example: { 1, 2, 3 }
        */
        class ArrayExpr : public Expr
        {
        public:
            ArrayExpr(const Utils::SourceLocation& srcLoc)
                : Expr{ NodeKind::ARRAY_EXPR }
            {
                mSrcLoc = srcLoc;
            }

            virtual ~ArrayExpr() = default;

        public:
            /*
            * \fn           AddArrayElement
            * \brief        Add an element to the array expression
            * \param elem   Element to be added
            */
            void AddArrayElement(std::unique_ptr<Expr>&& elem) { AddChildNode(std::move(elem)); }
        };

		/*
		* \class BinaryOpExpr
		* \brief Node of the AST representing a binary expression be it arithmetic (+, -, *, \, %) or logical (||, &&)
		*/
		class BinaryOpExpr : public Expr
		{
		public:
            BinaryOpExpr(Common::Operation op, std::unique_ptr<Expr>&& lhs, std::unique_ptr<Expr>&& rhs,
                const Utils::SourceLocation& srcLoc) :
                Expr{ NodeKind::BINARY_EXPR }, mOp{ op }
            {
                assert(lhs != nullptr);
                assert(rhs != nullptr);

                mSrcLoc = srcLoc;

                AddChildNode(std::move(lhs));
                AddChildNode(std::move(rhs));
            }

            virtual ~BinaryOpExpr() = default;

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
            const Common::Operation GetOperation() const { return mOp; }

            /*
            * \fn       GetRHS
            * \brief    Gets the right hand side of the binary expression
            * \return   Expression on the right side of the operation
            */
            const Expr* GetRHS() const { assert(mChildren.size() == 2); return GetChildNodeAs<Expr>(1); }

		private:
			Common::Operation mOp;     /*!< Operation applied in the binary expression */
		};

        /*
        * \class BooleanExpr
        * \brief Node of the AST representing a boolean literal
        */
        class BooleanExpr : public Expr
        {
        public:
            BooleanExpr(bool value, const Utils::SourceLocation& srcLoc) : Expr{ NodeKind::BOOLEAN_EXPR }, mValue{ value }
            { 
                mName = mValue ? "True" : "False"; 
                mSrcLoc = srcLoc;
            }

            virtual ~BooleanExpr() = default;

            virtual bool IsLiteral() const override { return true; }

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
            CallExpr(const std::string& fnName, std::vector<std::unique_ptr<Expr>>&& args, const Utils::SourceLocation srcLoc)
                : Expr{ NodeKind::CALL_EXPR }
            { 
                mName = fnName; 
                mSrcLoc = srcLoc;
                mChildren.insert(mChildren.end(), std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
            }
            
            virtual ~CallExpr() = default;

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
            IdentifierExpr(const std::string& value, const Utils::SourceLocation& srcLoc)
                : Expr{ NodeKind::IDENTIFIER_EXPR }, mType{ }
            {
                mName = value; 
                mSrcLoc = srcLoc;
            }
            virtual ~IdentifierExpr() = default;

        public:
            void SetType(const Common::Type type) { mType = type; }
            Common::Type GetType() const { return mType; }

        private:
            Common::Type mType; /*!< Associated variable type */
        };

        /*
        * \class IndexExpr
        * \brief Node of the AST representing an indexed value
        *        Example: tab[i]
        */
        class IndexExpr : public Expr
        {
        public:
            IndexExpr(std::unique_ptr<IdentifierExpr>&& arrayIdentifier, std::unique_ptr<Expr>&& indexExpr, const Utils::SourceLocation& srcLoc)
                : Expr{ NodeKind::INDEX_EXPR }
            {
                mName = arrayIdentifier->GetName();
                mSrcLoc = srcLoc;

                AddChildNode(std::move(arrayIdentifier));
                AddChildNode(std::move(indexExpr));
            }
            virtual ~IndexExpr() = default;
        };

        /*
        * \class NumberExpr
        * \brief Node of the AST representing a number literal
        */
        class NumberExpr : public Expr
        {
        public:
            NumberExpr(int value, const Utils::SourceLocation& srcLoc)
                : Expr{ NodeKind::NUMBER_EXPR }, mValue{ value } 
            { 
                mName = std::to_string(value); 
                mSrcLoc = srcLoc;
            }
            virtual ~NumberExpr() { }

            virtual bool IsLiteral() const override { return true; }
        
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

        /*
        * \class IdentifierExpr
        * \brief Node of the AST representing a string literal
        */
        class StringExpr : public Expr
        {
        public:
            StringExpr(const std::string& value, const Utils::SourceLocation& srcLoc)
                : Expr{ NodeKind::STRING_EXPR }
            {
                mName = value;
                mSrcLoc = srcLoc;
            }

            virtual ~StringExpr() = default;
        };
    }
}

#endif // EXPR_H__TOSTITOS
