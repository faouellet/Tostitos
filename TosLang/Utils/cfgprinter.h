#ifndef CFG_PRINTER_H__TOSTITOS
#define CFG_PRINTER_H__TOSTITOS

#include "../Common/cfgvisitor.h"

#include <iostream>

namespace TosLang
{
    namespace Utils
    {
        template <typename OS>
        struct PrintAction;

        template <>
        struct PrintAction<std::ostream>
        {
            void Execute(const BackEnd::BlockPtr block)
            {
                for (auto instIt = block->inst_begin(), instEnd = block->inst_end(); instIt != instEnd; ++instIt)
                {
                    std::cout << *instIt << std::endl;;
                }
            }
        };

        template <typename OS>
        using CFGPrinter = Common::CFGVisitor<PrintAction<OS>>;
    }
}

#endif //CFG_PRINTER_H__TOSTITOS