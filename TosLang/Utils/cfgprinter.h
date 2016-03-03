#ifndef CFG_PRINTER_H__TOSTITOS
#define CFG_PRINTER_H__TOSTITOS

#include "../Common/cfgvisitor.h"

namespace TosLang
{
    namespace Utils
    {
        template <typename OS>
        struct PrintAction 
        {
            void Execute(const BlockPtr block)
            {
                for (auto& inst : block.get())
                {
                    OS << inst << "\n";
                }
            }
        };

        template <typename OS>
        using CFGPrinter = Common::CFGVisitor<PrintAction<OS>>;
    }
}

#endif //CFG_PRINTER_H__TOSTITOS
