#include "thread.h"

#include "../machine/machine.h"

using namespace Threads;
using MachineEngine::ProcessorSpace::ThreadContext;

Thread::Thread(const std::string & Filename) : mFinished{false}, mWaitForChildren{false}, mTimeToWakeup{0}
{
	mContext = std::make_unique<ThreadContext>();
	// TODO: Should check if the ROM was correctly acquired
    MachineEngine::Machine::GetInstance().getInterpreter().AcquireROM(Filename);
}

bool Thread::IsWaitingForChildren()
{
	if (mWaitForChildren)
		mWaitForChildren = !mChildren.empty();

	return mWaitForChildren;
}

bool Thread::IsSleeping()
{
	if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - mTimePoint).count() > mTimeToWakeup)
	{
		mTimeToWakeup = 0;
		return false;
	}
	return true;
}

Thread * Thread::Fork(const std::string & Filename)
{
    Thread * Child = new Thread(Filename);
    mChildren.push_back(Child);
    return Child;
}

void Thread::Sleep(const unsigned int Time)
{
	mTimeToWakeup = Time;
	mTimePoint = std::chrono::high_resolution_clock::now();
}

void Thread::Barrier()
{
	mWaitForChildren = true;
}

