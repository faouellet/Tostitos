#ifndef ACTIVATION_RECORD_H__TOSTITOS
#define ACTIVATION_RECORD_H__TOSTITOS

#include <map>
#include <memory>
#include <vector>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
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
    }
}

#endif //ACTIVATION_RECORD_H__TOSTITOS
