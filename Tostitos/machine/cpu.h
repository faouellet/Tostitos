#ifndef CPU_H__TOSTITOS
#define CPU_H__TOSTITOS

#include "threadInfo.h"

#include <functional>
#include <memory>

namespace MachineEngine
{
    namespace ProcessorSpace
    {
        class CPU
        {
        private:
            std::shared_ptr<ThreadContext> mCurrentContext;
            std::vector<std::string> mStrings;
            std::string Filename;

        private:
            void Compute(std::function<unsigned int(unsigned int, unsigned int)> Func);

        public:
			CPU() = default;
			~CPU() = default;

            std::string GetFilename() const
            {
                return Filename;
            }

            bool AcquireROM(const std::string & FilePath, std::shared_ptr<ThreadContext> & Context);
            ThreadAction InterpretOne();

            void SwitchContext(std::shared_ptr<ThreadContext> & Context)
            {
                mCurrentContext = Context;
            }
        };
    }
}

#endif
