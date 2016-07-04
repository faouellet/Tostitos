#ifndef SCHEDULER_H__TOSTITOS
#define SCHEDULER_H__TOSTITOS

// TODO: Comments

#include <vector>

namespace Threading
{
    class Thread;
}

namespace KernelSpace
{
	class Scheduler
	{
	private:
		std::vector<Threading::Thread*> mThreadList;
        
    public:
		Threading::Thread* FindNextThreadToRun(Threading::Thread* RunningThread);
		void ScheduleThread(Threading::Thread* T);
		void TerminateThread(Threading::Thread* T);
	};
}

#endif