#include "thread.h"

using namespace Execution;
using namespace Threads;

namespace chr = std::chrono;

Thread::Thread(Executor&& exec) 
    : mFinished{ false }, mWaitForChildren{ false }, mTimeToWakeup{ 0 }, 
      mTimePoint{ }, mExecutor{ }, mChildren{ } { }

bool Thread::IsWaitingForChildren()
{
	if (mWaitForChildren)
		mWaitForChildren = !mChildren.empty();

	return mWaitForChildren;
}

bool Thread::IsSleeping()
{
	if (chr::duration_cast<chr::seconds>(chr::high_resolution_clock::now() - mTimePoint).count() > mTimeToWakeup)
	{
		mTimeToWakeup = 0;
		return false;
	}
	return true;
}

Thread* Thread::Fork(Executor&& exec)
{
    mChildren.emplace_back(std::make_unique<Thread>(std::move(exec)));
    return mChildren.back().get();
}

void Thread::Sleep(const unsigned int time)
{
	mTimeToWakeup = time;
	mTimePoint = std::chrono::high_resolution_clock::now();
}

void Thread::Barrier()
{
	mWaitForChildren = true;
}

