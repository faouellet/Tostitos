#ifndef ACTIVATION_RECORDER_H__TOSTITOS
#define ACTIVATION_RECORDER_H__TOSTITOS

#include "../CodeGen/activationrecord.h"
#include "../Common/astvisitor.h"
#include "../Sema/symboltable.h"

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class FunctionDecl;
        class SymbolTable;
    }

    namespace BackEnd
    {
        /*
        * \class ActivationRecorder 
        * \brief AST pass that collects activation information on all the functions of a program. It is assumed 
        *        to be run as the very first code generation pass just after transitionning from the semantic phases.
        */
        class ActivationRecorder : public Common::ASTVisitor<ActivationRecorder>
        {
            friend class Common::ASTVisitor<ActivationRecorder>;

        public:
            /*
            * \fn       ActivationRecorder
            * \brief    Ctor
            */
            ActivationRecorder();

        public:
            /*
            * \fn           Run
            * \brief        Recursively walk the tree rooted at root to collect activation record information
            * \param root   Root of the AST
            * \param symTab Symbol table associated with the given AST
            * \return       The activation records for all the functions in a program
            */
            FuncRecords&& Run(const std::unique_ptr<FrontEnd::ASTNode>& root, const std::shared_ptr<FrontEnd::SymbolTable>& symTab);
        
        protected:
            void HandleFunctionDecl();
            void HandleVarDecl();

        private:
            size_t mNbLocalVars;                                    /*!< Number of local variables that a function has */
            RecordPtr mCurrentRecord;                               /*!< Current activation record being built */
            FuncRecords mFuncRecords;                               /*!< Activation records a program's functions */
            FrontEnd::FunctionDecl* mCurrentFunc;                   /*!< Current traversed function */
            std::shared_ptr<FrontEnd::SymbolTable> mSymbolTable;    /*!< Symbol table to be used by the type checker */
        };
    }
}

#endif // ACTIVATION_RECORDER_H__TOSTITOS
