#include "kernel.h"

#include "scheduler.h"
#include "../threading/executor.h"
#include "../threading/thread.h"

#include "../../TosLang/AST/ast.h"
#include "../../TosLang/Execution/compiler.h"

using namespace KernelSpace;
using namespace Threading;

Kernel::Kernel() = default;
Kernel::~Kernel() = default;

Kernel& Kernel::GetInstance()
{
    static Kernel Instance;
    return Instance;
}

void Kernel::RunProgram(const std::string& programName)
{
    Execution::Compiler compiler;

    mRoot = compiler.ParseProgram(programName);

    if (mRoot != nullptr)
    {
        mSymTable = compiler.GetSymbolTable(mRoot);

        Threading::impl::Executor mainExec{ mRoot.get(), mSymTable.get() };

        auto mainThread = std::make_unique<Thread>(std::move(mainExec));

        mScheduler.ScheduleThread(mThreads.back().get());
    }

    Run();
}

void Kernel::AddThread(std::unique_ptr<Thread>&& thread)
{
    // Take ownership of the thread
    mThreads.emplace_back(std::move(thread));

    // And then schedule it to run
    mScheduler.ScheduleThread(mThreads.back().get());
}

void Kernel::SleepFor(size_t nbSecs)
{
    mCurrentThread->Sleep(nbSecs);
}

void Kernel::Sync()
{
    mCurrentThread->Barrier();
}

void Kernel::Run()
{
    mCurrentThread = mScheduler.FindNextThreadToRun(mCurrentThread);

    while (!mCurrentThread->HasFinished())
    {
        mCurrentThread->ExecuteOne();

        if (mCurrentThread->IsSleeping() || mCurrentThread->IsWaitingForChildren())
        {
            mCurrentThread = mScheduler.FindNextThreadToRun(mCurrentThread);
        }
    }

    // TODO: Clean up the rest of the threads?
}
