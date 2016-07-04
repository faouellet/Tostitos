#ifndef KERNEL_H__TOSTITOS
#define KERNEL_H__TOSTITOS

// TODO: Comments

#include "scheduler.h"
#include <memory>

namespace Threading
{
    class Thread;
}

namespace KernelSpace
{
	class Kernel
	{
    public:
        ~Kernel();

    public:
        static Kernel& GetInstance();

	public:
        void Run(const std::string & programName);

    public:
        //void CreateThread(Execution::Executor&& exec);
        void SleepFor(size_t nbSecs);
        void Sync();

    private:
        Kernel();
        Kernel(const Kernel&) = delete;
        void operator=(const Kernel&) = delete;

    private:
        Threading::Thread* mCurrentThread;
        std::vector<std::unique_ptr<Threading::Thread>> mThreads;
        Scheduler mScheduler;
	};
}

#endif
