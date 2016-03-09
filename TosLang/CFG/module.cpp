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

void Module::Print() const
{
    // Print global variables
    for (auto gVarIt = mGlobalBlock->inst_begin(), gVarEnd = mGlobalBlock->inst_end(); gVarIt != gVarEnd; ++gVarIt)
        std::cout << *gVarIt << std::endl;

    // Print functions' cfgs
    Utils::CFGPrinter<std::ostream> printer;
    for (auto& funcCFG : mFuncCFGs)
    {
        std::cout << "CFG for " << funcCFG.first << ":\n";
        printer.Visit(funcCFG.second, /*postOrderVisit=*/false);
        std::cout << std::endl;
    }
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

