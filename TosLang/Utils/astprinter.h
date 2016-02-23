#ifndef AST_PRINTER_H__TOSTITOS
#define AST_PRINTER_H__TOSTITOS

#include "../Common/astvisitor.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"

namespace TosLang
{
    namespace Utils
    {
        /*
        * \class
        * \brief Prints the AST the output stream specified as the template parameter
        */
        template <typename OS>
        class ASTPrinter : public Common::ASTVisitor<ASTPrinter<OS>>
        {
            friend class Common::ASTVisitor<ASTPrinter<OS>>;

        public:
            explicit ASTPrinter(OS&& stream) : mCurrentLevel{ 0 }, mStream(std::move(stream))
            {
                this->mPrologueFtr = [this]() { ++mCurrentLevel; };
                this->mEpilogueFtr = [this]() { --mCurrentLevel; };
            }
            
            ~ASTPrinter() = default;

        public:
            /*
            * \fn           Print
            * \param root   Root of the tree to print
            * \brief        Recursively prints the tree rooted at root.
            */
            void Run(const std::unique_ptr<FrontEnd::ASTNode>& root)
            {
                this->VisitPreOrder(root);
            }

        protected:  // Declarations
            /*
            * \fn       HandleProgramDecl
            * \brief    Prints a node of the PROGRAM_DECL kind
            */
            void HandleProgramDecl()
            {
                Indent();
                mStream << "ProgramDecl\n";
            }

            /*
            * \fn       HandleFunctionDecl
            * \brief    Prints a node of the FUNCTION_DECL kind
            */
            void HandleFunctionDecl()
            {
                const FrontEnd::FunctionDecl* fDecl = dynamic_cast<const FrontEnd::FunctionDecl*>(this->mCurrentNode);
                assert(fDecl != nullptr);

                Indent();
                mStream << "FunctionDecl: " << fDecl->GetName()
                        << " Return Type: " << static_cast<std::underlying_type<Common::Type>::type>(fDecl->GetReturnType())
                        << "\n";
            }

            /*
            * \fn       HandleVarDecl
            * \brief    Prints a node of the VAR_DECL kind
            */
            void HandleVarDecl()
            {
                const FrontEnd::VarDecl* vDecl = dynamic_cast<const FrontEnd::VarDecl*>(this->mCurrentNode);
                assert(vDecl != nullptr);

                Indent();
                mStream << "VarDecl: " << vDecl->GetVarName() 
                        << " Type: " << static_cast<std::underlying_type<Common::Type>::type>(vDecl->GetVarType()) 
                        << "\n";
            }

        protected:  // Expressions
            /*
            * \fn       HandleBinaryOpExpr
            * \brief    Prints a node of the BINARY_EXPR kind
            */
            void HandleBinaryOpExpr()
            {
                const FrontEnd::BinaryOpExpr* bExpr = dynamic_cast<const FrontEnd::BinaryOpExpr*>(this->mCurrentNode);
                assert(bExpr != nullptr);

                Indent();
                mStream << "BinaryOpExpr: " << bExpr->GetOperation() << "\n";
            }

            /*
            * \fn       HandleNumberExpr
            * \brief    Prints a node of the BOOLEAN_EXPR kind
            */
            void HandleBooleanExpr()
            {
                const FrontEnd::BooleanExpr* bExpr = dynamic_cast<const FrontEnd::BooleanExpr*>(this->mCurrentNode);
                assert(bExpr != nullptr);

                Indent();
                mStream << "BooleanExpr: ";

                if (bExpr->GetValue())
                    mStream << "True";
                else
                    mStream << "False";

                mStream << "\n";
            }

            /*
            * \fn       HandleCallExpr
            * \brief    Prints a node of the CALL_EXPR kind
            */
            void HandleCallExpr()
            {
                const FrontEnd::CallExpr* cExpr = dynamic_cast<const FrontEnd::CallExpr*>(this->mCurrentNode);
                assert(cExpr != nullptr);

                Indent();
                mStream << "CallExpr: " << cExpr->GetCalleeName() << "\n";
            }

            /*
            * \fn       HandleNumberExpr
            * \brief    Prints a node of the NUMBER_EXPR kind
            */
            void HandleNumberExpr()
            {
                const FrontEnd::NumberExpr* nExpr = dynamic_cast<const FrontEnd::NumberExpr*>(this->mCurrentNode);
                assert(nExpr != nullptr);

                Indent();
                mStream << "NumberExpr: " << nExpr->GetValue() << "\n";
            }

            /*
            * \fn       HandleStringExpr
            * \brief    Prints a node of the STRING_EXPR kind
            */
            void HandleStringExpr()
            {
                const FrontEnd::StringExpr* sExpr = dynamic_cast<const FrontEnd::StringExpr*>(this->mCurrentNode);
                assert(sExpr != nullptr);

                Indent();
                mStream << "StringExpr: " << sExpr->GetName() << "\n";
            }

        protected:  // Statements
            /*
            * \fn       HandleCompoundStmt
            * \brief    Prints a node of the COMPOUND_STMT kind
            */
            void HandleCompoundStmt()
            {
                Indent();
                mStream << "CompoundStmt\n";
            }

            /*
            * \fn       HandleIfStmt
            * \brief    Prints a node of the IF_STMT kind
            */
            void HandleIfStmt()
            {
                Indent();
                mStream << "IfStmt\n";
            }

            /*
            * \fn       HandlePrintStmt
            * \brief    Prints a node of the PRINT_STMT kind
            */
            void HandlePrintStmt()
            {
                Indent();
                mStream << "PrintStmt\n";
            }

            /*
            * \fn       HandleReturnStmt
            * \brief    Prints a node of the RETURN_STMT kind
            */
            void HandleReturnStmt()
            {
                Indent();
                mStream << "ReturnStmt\n";
            }

            /*
            * \fn       HandleScanStmt
            * \brief    Prints a node of the SCAN_STMT kind
            */
            void HandleScanStmt()
            {
                Indent();
                mStream << "ScanStmt\n";
            }

            /*
            * \fn       HandleWhileStmt
            * \brief    Prints a node of the WHILE_STMT kind
            */
            void HandleWhileStmt()
            {
                Indent();
                mStream << "WhileStmt\n";
            }

        private:
            /*
            * \fn       Indent
            * \brief    Indent the text to print according to the current level in the traversal
            */
            void Indent()
            {
                for (unsigned i = 0; i < this->mCurrentLevel - 1; ++i)
                    mStream << "\t";
            }

        private:
            size_t mCurrentLevel;   /*!< Current tree level of the traversal */
            OS mStream;             /*!< Output stream to which the AST will be printed */
        };

    }
}

#endif // AST_PRINTER_H__TOSTITOS
