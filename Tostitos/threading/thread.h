#ifndef THREAD_H__TOSTITOS
#define THREAD_H__TOSTITOS

// TODO: Comments

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Threading
{
    namespace impl
    {
        class Executor;
    }

	class Thread
	{
	public:
		explicit Thread(impl::Executor&& exec);
        ~Thread();

        void ExecuteOne();
		bool HasFinished() const { return mFinished; }
		bool IsWaitingForChildren();
		bool IsSleeping();
        
		Thread* Fork(impl::Executor&& exec);
		void Sleep(size_t Time);
		void Barrier();
        
    private:
        bool mFinished;
        bool mWaitForChildren;
        size_t mTimeToWakeup;
        std::chrono::high_resolution_clock::time_point mTimePoint;

        std::unique_ptr<impl::Executor> mExecutor;

        std::vector<std::unique_ptr<Thread>> mChildren;
	};
}

#endif // THREAD_H__TOSTITOS
