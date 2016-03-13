#ifndef ACTIVATION_RECORDER_H__TOSTITOS
#define ACTIVATION_RECORDER_H__TOSTITOS

#include "../Common/astvisitor.h"
#include "../Sema/symboltable.h"

#include <map>

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
        * \class ActivationRecord
        * \brief
        */
        class ActivationRecord
        {
        public:
            ActivationRecord(size_t argSize = 0) 
                : mArgsSize{ argSize } { }

        public:
            void AddLocalVar(const FrontEnd::ASTNode* node) { mLocalVars.push_back(node); }
            const std::vector<const FrontEnd::ASTNode*>& GetLocalVar() const { return mLocalVars; }

        private:
            size_t mArgsSize;                                   /*!< Size of the memory occupied by the function's arguments */
            std::vector<const FrontEnd::ASTNode*> mLocalVars;   /*!< Local variables ordered by creation order. 
                                                                     This will allow fast computation of the local variables' memory location */
        };

        using RecordPtr = std::unique_ptr<ActivationRecord>;
        using FuncRecords = std::map<std::string, RecordPtr>;

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
