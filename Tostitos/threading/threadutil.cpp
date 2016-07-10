#include "threadutil.h"

#include "executor.h"
#include "thread.h"

#include "../kernel/kernel.h"
#include "../kernel/scheduler.h"

#include "../../TosLang/AST/ast.h"
#include "../../TosLang/Sema/symboltable.h"

using namespace KernelSpace;
using namespace Threading::impl;
using namespace TosLang::FrontEnd;

namespace Threading
{
    void CreateThread(const ASTNode* root, const SymbolTable* symTab)
    {
        // Create a call stack for the main thread then push the (empty for now) global 
        // frame onto it. This frame will contains all global scope level informations 
        // that are available to all the functions in the program. This make it the only
        // frame that can be accessed by any other.
        CallStack cStack;
        cStack.PushFrame({});
    
        // Create the execution agent
        Executor exec{ root, symTab, std::move(cStack) };
    
        // Create the thread on which the execution agent will run
        auto thread = std::make_unique<Thread>(std::move(exec));
    
        // Give the thread to the kernel so it can be executed
        Kernel::GetInstance().AddThread(std::move(thread));
    }
        
    void CurrentThreadSleepFor(size_t nbSecs)
    {
        Kernel::GetInstance().SleepFor(nbSecs);
    }
    
    void CurrentThreadSync()
    {
        Kernel::GetInstance().Sync();
    }
}
