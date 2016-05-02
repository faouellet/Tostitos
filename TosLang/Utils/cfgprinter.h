#ifndef CFG_PRINTER_H__TOSTITOS
#define CFG_PRINTER_H__TOSTITOS

#include "../Common/cfgvisitor.h"

#include <iostream>

namespace TosLang
{
    namespace Utils
    {
        template <class InstT, class OS>
        struct PrintAction
        {
            explicit PrintAction(OS& stream) : mStream{ stream } { }

            void Execute(const BackEnd::BlockPtr<InstT> block)
            {
                mStream << block->GetName() << std::endl;

                for (auto instIt = block->inst_begin(), instEnd = block->inst_end(); instIt != instEnd; ++instIt)
                {
                    mStream << "\t" << **instIt << std::endl;;
                }

                mStream << std::endl;
            }

        private:
            OS& mStream;
        };

        template <class OS, class InstT>
        using CFGPrinter = Common::CFGVisitor<InstT, PrintAction<InstT, OS>>;
    }
}

#endif //CFG_PRINTER_H__TOSTITOS
