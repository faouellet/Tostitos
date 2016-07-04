#include "scheduler.h"

#include "../threading/thread.h"

#include <algorithm>

using namespace KernelSpace;
using namespace Threading;

Thread* Scheduler::FindNextThreadToRun(Thread* RunningThread)
{
	if (!RunningThread->HasFinished() && !RunningThread->IsWaitingForChildren() && !RunningThread->IsSleeping())
	{
		return RunningThread;
	}
	else if (RunningThread->HasFinished())
	{
		mThreadList.erase(std::find(mThreadList.begin(), mThreadList.end(), RunningThread));
	}

	if (!mThreadList.empty())
	{
		Thread* NewRunningThread = *mThreadList.begin();
		// Machine::GetInstance().getCpu().SwitchContext(NewRunningThread->DumpContext());
        return NewRunningThread;
	}
	else
	{
		return nullptr;
	}
}

void Scheduler::ScheduleThread(Thread * T)
{
	mThreadList.push_back(T);
}

void Scheduler::TerminateThread(Thread* T)
{
    mThreadList.erase(std::remove(mThreadList.begin(), mThreadList.end(), T));
}
