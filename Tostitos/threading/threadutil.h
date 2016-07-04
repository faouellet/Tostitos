#ifndef THREAD_UTIL_H__TOSTITOS
#define THREAD_UTIL_H__TOSTITOS

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
    void Fork(const TosLang::FrontEnd::ASTNode* root,
              const TosLang::FrontEnd::SymbolTable* symTab);
    void SleepFor(size_t nbSecs);
    void Sync();
}

#endif // THREAD_UTIL_H__TOSTITOS
