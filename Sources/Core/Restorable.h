#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Restorable : public RedLiliumObject
{
public:
	Restorable() {}

private:

};

template<class T>
using rptr = T*;

} // RED_LILIUM_NAMESPACE
