#include "activationrecorder.h"

#include "../AST/declarations.h"

using namespace TosLang::BackEnd;
using namespace TosLang::FrontEnd;

ActivationRecorder::ActivationRecorder() 
    : mNbLocalVars { 0 }, mCurrentRecord{ nullptr }, mFuncRecords{},
      mCurrentFunc{ nullptr }, mSymbolTable{ nullptr }
{
    this->mPrologueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            mCurrentFunc = dynamic_cast<FunctionDecl*>(mCurrentNode);
            assert(mCurrentFunc != nullptr);

            mCurrentRecord.reset(new ActivationRecord{});
        }
    };

    this->mEpilogueFtr = [this]()
    {
        if (mCurrentNode->GetKind() == ASTNode::NodeKind::FUNCTION_DECL)
        {
            mCurrentFunc = dynamic_cast<FunctionDecl*>(mCurrentNode);
            assert(mCurrentFunc != nullptr);

            auto res = mFuncRecords.emplace(mCurrentFunc->GetName(), std::move(mCurrentRecord));
            assert(res.second);
        }
    };
}

FuncRecords&& ActivationRecorder::Run(const std::unique_ptr<ASTNode>& root, const std::shared_ptr<SymbolTable>& symTab)
{
    // Reset the pass state
    mSymbolTable = symTab;
    mFuncRecords.clear();

    this->VisitPostOrder(root);

    return std::move(mFuncRecords);
}

void ActivationRecorder::HandleFunctionDecl()
{
    const FunctionDecl* fDecl = dynamic_cast<FunctionDecl*>(mCurrentNode);
    assert(fDecl != nullptr);

    mCurrentRecord.reset(new ActivationRecord{ fDecl->GetParametersDecl()->GetParameters().size() });
}

void ActivationRecorder::HandleVarDecl()
{
    const VarDecl* vDecl = dynamic_cast<VarDecl*>(mCurrentNode);
    assert(vDecl != nullptr);

    if (!mSymbolTable->IsGlobalVariable(vDecl->GetName()))
        mCurrentRecord->AddLocalVar(mCurrentNode);
}
