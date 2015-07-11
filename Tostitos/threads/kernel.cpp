#include "kernel.h"
#include "scheduler.h"

#include "../machine/machine.h"

using namespace Threads;
using MachineEngine::ProcessorSpace::ThreadAction;

void Kernel::Init(const std::string & ProgramName)
{
    mCurrentThread = new Thread(ProgramName);
	// Scheduler::GetInstance().ScheduleThread(mCurrentThread);
	// MachineEngine::Machine::GetInstance().getCpu().SwitchContext(mCurrentThread->DumpContext());
}

void Kernel::Run()
{

	//ThreadAction currentAction;
	//while (mCurrentThread != nullptr)
	//{
	//	if (mCurrentThread->IsSleeping())
	//	{
	//		mCurrentThread = Scheduler::GetInstance().FindNextThreadToRun(mCurrentThread);
	//	}
	//	else
	//	{
	//		currentAction = MachineEngine::Machine::GetInstance().getCpu().InterpretOne();

	//		switch (currentAction)
	//		{
    //          // TODO
	//			default:
	//			{
	//				break;
	//			}
	//		}
	//	}
	//}
}
