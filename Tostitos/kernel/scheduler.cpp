#include "scheduler.h"

#include "../threading/thread.h"

#include <algorithm>

using namespace KernelSpace;
using namespace Threading;

Thread* Scheduler::FindNextThreadToRun(Thread* runningThread)
{
    if (runningThread != nullptr)
    {
        // If it's not done and not stopped, keep going
        if (!runningThread->HasFinished() &&
            !runningThread->IsWaitingForChildren() &&
            !runningThread->IsSleeping())
        {
            return runningThread;
        }

        // If it's done, remove it from the list
        if (runningThread->HasFinished())
        {
            mThreadList.erase(std::find(mThreadList.begin(), mThreadList.end(), runningThread));
        }
    }

    // The next thread to run will simply be the first one in our list
	if (!mThreadList.empty())
        return mThreadList.front();
	else
		return nullptr;
}

void Scheduler::ScheduleThread(Thread* thread)
{
	mThreadList.push_back(thread);
}

void Scheduler::TerminateThread(Thread* thread)
{
    mThreadList.erase(std::remove(mThreadList.begin(), mThreadList.end(), thread));
}
