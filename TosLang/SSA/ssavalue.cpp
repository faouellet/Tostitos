#include "ssavalue.h"

#include <cassert>

using namespace TosLang::BackEnd;

//////////////////// SSAValue ////////////////////

std::ostream& TosLang::BackEnd::operator<<(std::ostream& stream, const SSAValue& ssaVal)
{
    assert(ssaVal.mKind != SSAValue::ValueKind::UNKNOWN);

    if (ssaVal.mKind == SSAValue::ValueKind::ARGUMENT)
    {
        return stream << "V" << ssaVal.mID;
    }
    else   // LITERAL
    {
        return stream << ssaVal.mLitVal;
    }
}

bool TosLang::BackEnd::operator==(const SSAValue& lhsVal, const SSAValue& rhsVal)
{
    return lhsVal.mID == rhsVal.mID
        && lhsVal.mDef == rhsVal.mDef
        && lhsVal.mKind == rhsVal.mKind
        && lhsVal.mLitVal == rhsVal.mLitVal;
}

bool TosLang::BackEnd::operator!=(const SSAValue& lhsVal, const SSAValue& rhsVal)
{
    return !(lhsVal == rhsVal);
}
