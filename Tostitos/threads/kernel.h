#ifndef KERNEL_H__TOSTITOS
#define KERNEL_H__TOSTITOS

#include "thread.h"

namespace Threads
{
	class Kernel
	{
	private:
		Thread * mCurrentThread;

	public:
		Kernel() = default;
		~Kernel() = default;

        void Init(const std::string & ProgramName);
		void Run();
	};
}

#endif
