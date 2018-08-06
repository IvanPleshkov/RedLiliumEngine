#include "pch.h"
#include "BgfxEnvironment.h"

using namespace RED_LILIUM_NAMESPACE;

Time BgfxEnvironment::GetDoubleClickingTime() const
{
	return Time(0.3);
}

std::string BgfxEnvironment::GetUserInputSymbol() const
{
	return std::string();
}
