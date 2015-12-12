#ifndef INTERNAL_HELPER_FUNCTIONS_H__TOSTITOS
#define INTERNAL_HELPER_FUNCTIONS_H__TOSTITOS

#include <chrono>
#include <thread>

namespace MachineEngine
{
    namespace InternalFunctions
    {
        template <class L>
        void internalTimer(std::chrono::milliseconds timeToWait, L func)
        {
            std::thread([=]
            {
                std::this_thread::sleep_for(timeToWait);
                func();
            }).detach();
        }
    }
}

#endif