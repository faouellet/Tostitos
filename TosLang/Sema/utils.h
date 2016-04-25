#ifndef SEMA_UTILS_H__TOSTITOS
#define SEMA_UTILS_H__TOSTITOS

#include "../Common/type.h"

#include <unordered_map>

namespace TosLang
{
    namespace FrontEnd
    {
        class ASTNode;
        
        using NodeTypeMap = std::unordered_map<const ASTNode*, Common::Type>;
    }
}

#endif // SEMA_UTILS_H__TOSTITOS
