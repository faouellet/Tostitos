#ifndef THREAD_H__TOSTITOS
#define THREAD_H__TOSTITOS

#include "../machine/threadInfo.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Threads
{
	class Thread
	{
	private:
		bool mFinished;
		bool mWaitForChildren;
		unsigned int mTimeToWakeup;
		std::chrono::high_resolution_clock::time_point mTimePoint;

        std::shared_ptr<MachineEngine::ProcessorSpace::ThreadContext> mContext;
		std::vector<Thread*> mChildren;

	public:
		Thread(const std::string & Filename);
		~Thread() = default;

		void Finish() { mFinished = true; }
		bool HasFinished() const { return mFinished; }
		bool IsWaitingForChildren();
		bool IsSleeping();

        std::shared_ptr<MachineEngine::ProcessorSpace::ThreadContext> & DumpContext()
        {
            return mContext;
        }

		Thread * Fork(const std::string &Filename);
		void Sleep(const unsigned int Time);
		void Barrier();
	};
}

#endif
