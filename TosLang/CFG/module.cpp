#include "module.h"

#include "../Utils/cfgprinter.h"

using namespace TosLang::BackEnd;

//////////////////// Module ////////////////////

Module::Module() : mCurrentMemorySlot{ 0 }
{
    mGlobalBlock.reset(new BasicBlock{"GLOBAL"});
}

const CFGPtr& Module::GetFunction(const std::string& name) const
{
    return mFuncCFGs.at(name);
}

unsigned Module::InsertArrayVariable(const std::string& name, const std::string& value)
{
    mArrayVars.emplace(name, std::make_unique<ArrayVar<unsigned char>>(value));

    return mCurrentMemorySlot++;
}

void Module::InsertFunction(const std::string& name, const CFGPtr& cfg)
{
    mFuncCFGs[name] = cfg;
}

void Module::InsertGlobalVar(const VirtualInstruction& inst)
{
    mGlobalBlock->InsertInstruction(inst);
}
