#ifndef KERNEL_H__TOSTITOS
#define KERNEL_H__TOSTITOS

// TODO: Comments

#include "scheduler.h"
#include <memory>

namespace Threading
{
    class Thread;
}

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class SymbolTable;
    }
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
        void RunProgram(const std::string& programName);

    public:
        void AddThread(std::unique_ptr<Threading::Thread>&& thread);
        void SleepFor(size_t nbSecs);
        void Sync();

    private:
        Kernel();
        Kernel(const Kernel&) = delete;
        void operator=(const Kernel&) = delete;

    private:
        void Run();

    private:
        Threading::Thread* mCurrentThread;
        std::vector<std::unique_ptr<Threading::Thread>> mThreads;
        Scheduler mScheduler;

    private:    // TODO: Temporarily put there.
        std::unique_ptr<TosLang::FrontEnd::ASTNode> mRoot;
        std::shared_ptr<TosLang::FrontEnd::SymbolTable> mSymTable;
	};
}

#endif
