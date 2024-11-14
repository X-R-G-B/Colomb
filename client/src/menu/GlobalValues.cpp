#include "GlobalValues.hpp"

GlobalValues &GlobalValues::GetInstance()
{
    static GlobalValues GlobalValuesInternal;
    return GlobalValuesInternal;
}
