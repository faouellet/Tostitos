#include "module.h"

using namespace TosLang::BackEnd;

const CFGPtr& TosLang::BackEnd::Module::GetFunction(const std::string& name) const
{
    return mFuncCFGs.at(name);
}

void TosLang::BackEnd::Module::InsertFunction(const std::string& name, const CFGPtr& cfg)
{
    mFuncCFGs[name] = cfg;
}

void TosLang::BackEnd::Module::InsertGlobalVariable()
{
}