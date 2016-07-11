#ifndef THREAD_UTIL_H__TOSTITOS
#define THREAD_UTIL_H__TOSTITOS

#include <functional>

#if defined(__unix__) || defined(__APPLE__)
#include <cstddef>
#endif

// TODO: Comments

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        class SymbolTable;
    }
}

namespace Threading
{
    namespace impl
    {
        class InterpretedValue;
    }

    void CreateThread(const TosLang::FrontEnd::ASTNode* root,
                      const TosLang::FrontEnd::SymbolTable* symTab,
                      std::function<void(impl::InterpretedValue)>&& fn);
    void CurrentThreadSleepFor(size_t nbSecs);
    void CurrentThreadSync();
}

#endif // THREAD_UTIL_H__TOSTITOS
