#pragma once

#include <entry/entry.h> // bgfx
#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{

class BgfxWindow : public RedLiliumObject, public INativeWindow
{
public:
	BgfxWindow();
	~BgfxWindow() override {}

	void UpdateWindowState(const entry::WindowState& windowState, NVGcontextPtr nvgContext);

	vec2 GetSize() const override;
	bool IsFocused() const override;
	NVGcontextPtr GetNvgContext() override;
	const NVGcontextPtr GetNvgContext() const override;

private:
	NVGcontextPtr m_nvgContext;
	entry::WindowState m_windowState;
};

} // namespace RED_LILIUM_NAMESPACE
