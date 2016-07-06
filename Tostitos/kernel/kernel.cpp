#include "kernel.h"

#include "scheduler.h"
#include "../threading/thread.h"

using namespace KernelSpace;
using namespace Threading;

Kernel::Kernel() = default;
Kernel::~Kernel() = default;

Kernel& Kernel::GetInstance()
{
    static Kernel Instance;
    return Instance;
}

void Kernel::Run(const std::string& programName)
{
    //ThreadAction currentAction;
	//while (mCurrentThread != nullptr)
	//{
	//	if (mCurrentThread->IsSleeping())
	//	{
	//		mCurrentThread = Scheduler::GetInstance().FindNextThreadToRun(mCurrentThread);
	//	}
	//	else
	//	{
	//		currentAction = MachineEngine::Machine::GetInstance().getCpu().InterpretOne();

	//		switch (currentAction)
	//		{
    //          // TODO
	//			default:
	//			{
	//				break;
	//			}
	//		}
	//	}
	//}
}

void Kernel::RunThread(std::unique_ptr<Thread>&& thread)
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
