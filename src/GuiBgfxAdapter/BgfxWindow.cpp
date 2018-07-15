#include "pch.h"
#include "BgfxWindow.h"

using namespace RED_LILIUM_NAMESPACE;

BgfxWindow::BgfxWindow()
	: m_nvgContext(nullptr)
	, m_windowState()
{
}

void BgfxWindow::UpdateWindowState(const entry::WindowState& windowState, NVGcontextPtr nvgContext)
{
	m_nvgContext = nvgContext;
	m_windowState = windowState;
}

vec2 BgfxWindow::GetSize() const
{
	return { m_windowState.m_width, m_windowState .m_height };
}

bool BgfxWindow::IsFocused() const
{
	return true;
}

NVGcontextPtr BgfxWindow::GetNvgContext()
{
	return m_nvgContext;
}

const NVGcontextPtr BgfxWindow::GetNvgContext() const
{
	return m_nvgContext;
}
