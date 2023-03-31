#pragma once
#include <math.h>

//Might add "epsilon * std::max(fabs(a), fabs(b)));"
namespace FloatCompare
{
    inline bool IsEqual(float a, float b, float epsilon) { return fabs(a - b) < epsilon; }
}