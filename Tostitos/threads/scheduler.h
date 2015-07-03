#ifndef SCHEDULER_H__TOSTITOS
#define SCHEDULER_H__TOSTITOS

#include "thread.h"

#include <vector>

namespace Threads
{
	class Scheduler
	{
	private:
		std::vector<Thread*> mThreadList;

	private:
		Scheduler() = default;
		Scheduler(const Scheduler &) = delete;
		void operator=(const Scheduler &) = delete;

	public:
		static Scheduler & GetInstance()
		{
			static Scheduler Instance;
			return Instance;
		}

		Thread * FindNextThreadToRun(Thread * RunningThread);
		void ScheduleThread(Thread * T);
		void TerminateThread(Thread * T);
	};
}

#endif