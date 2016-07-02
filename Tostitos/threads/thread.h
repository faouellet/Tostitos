#ifndef THREAD_H__TOSTITOS
#define THREAD_H__TOSTITOS

// TODO: Comments

#include "../../TosLang/Execution/executor.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Threads
{
	class Thread
	{
	public:
		Thread(Execution::Executor&& exec);

		void Finish() { mFinished = true; }
		bool HasFinished() const { return mFinished; }
		bool IsWaitingForChildren();
		bool IsSleeping();
        
		Thread* Fork(Execution::Executor&& exec);
		void Sleep(const unsigned int Time);
		void Barrier();

    private:
        bool mFinished;
        bool mWaitForChildren;
        unsigned int mTimeToWakeup;
        std::chrono::high_resolution_clock::time_point mTimePoint;

        std::unique_ptr<Execution::Executor> mExecutor;

        std::vector<std::unique_ptr<Thread>> mChildren;
	};
}

#endif
