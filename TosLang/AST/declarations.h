#ifndef DECL_H__TOSTITOS
#define DECL_H__TOSTITOS

#include "ast.h"
#include "expressions.h"

#include <cassert>
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
            ProgramDecl() : ASTNode(PROGRAM_DECL) { }
            virtual ~ProgramDecl() { }
        };

        /*
        * \class VarDecl
        * \brief Node of the AST representing a variable declaration.
        *        For example: var MyIntVar: Int
        */
        class VarDecl : public ASTNode
        {
        public:
            explicit VarDecl(const std::string& varName) : ASTNode(VAR_DECL) { mName = varName; }
            virtual ~VarDecl() { }

		public:
            void AddInitialization(std::unique_ptr<Expr>&& value) { mChildren.push_back(std::move(value)); }

            const std::unique_ptr<ASTNode>& GetInitExpr() const 
			{ 
				if (mChildren.empty())
				{
					return nullptr;
				}
				else
				{
					assert(mChildren.size() == 1);
					return mChildren[0];
				}
			}

            const std::string& GetVarName() const { return mName; }
        };
    }
}

#endif // DECL_H__TOSTITOS
