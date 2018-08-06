#pragma once

#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{

class BgfxEnvironment : public RedLiliumObject, public INativeEnvironment
{
public:
	BgfxEnvironment() {}
	~BgfxEnvironment() override {}

	Time GetDoubleClickingTime() const override;

	std::string GetUserInputSymbol() const override;
};

}  // namespace RED_LILIUM_NAMESPACE
