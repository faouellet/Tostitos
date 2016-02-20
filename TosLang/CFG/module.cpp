#include "module.h"

using namespace TosLang::BackEnd;

//////////////////// Module ////////////////////

const CFGPtr& Module::GetFunction(const std::string& name) const
{
    return mFuncCFGs.at(name);
}

void Module::InsertFunction(const std::string& name, const CFGPtr& cfg)
{
    mFuncCFGs[name] = cfg;
}

unsigned Module::InsertArrayVariable(const std::string& name, const std::string& value)
{
    mArrayVars.emplace(name, std::make_unique<ArrayVar<unsigned char>>(value));

    return mCurrentMemorySlot++;
}
