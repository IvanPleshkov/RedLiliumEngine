#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class GuiManager;

enum class GuiPipelineStep : u32
{
	Idle					= 1 << 0,
	AddingsWidgets			= 1 << 1,
	TickWidgets				= 1 << 2,
	SetWidgetsMinimumSize	= 1 << 3,
	SetWidgetsDesiredSize	= 1 << 4,
	SetWidgetsTransform		= 1 << 5,
	Drawing					= 1 << 6,
	HandlingMouseEvent		= 1 << 7,
	HandlingKeyEvent		= 1 << 8,
};

class Panel : public RedLiliumObject
{
public:
	friend class GuiManager;

	Panel(const ptr<GuiManager>& guiManager);
	~Panel() override {}

public:
	ptr<Widget> BeginFrame();
	void EndFrame(NVGcontextPtr nvgContext, vec2 position, vec2 size);

	NVGcontextPtr GetNvgContext();
	ptr<GuiManager> GetGuiManager();
	ptr<const GuiManager> GetGuiManager() const;
	void Draw();
	ptr<Widget> GetWidgetUnderPoint(vec2 position);
	GuiPipelineStep GetCurrentPipelineStep() const;

private:
	void DrawRecursive(const ptr<Widget>& widget);
	void UpdateWidgetsDesiredSize(const uptr<Widget>& widget);
	void UpdateWidgetsMinimumSize(const uptr<Widget>& widget);
	void UpdateWidgetsLayouting(const uptr<Widget>& widget);
	void UpdateWidgets(const uptr<Widget>& widget);
	ptr<Widget> GetWidgetUnderPoint(const uptr<Widget>& widget, vec2 position);
	void SetWidgetScissor(const ptr<Widget>& widget) const;

	GuiPipelineStep m_currentGuiPipelineState;
	ptr<GuiManager> m_guiManager;
	uptr<Widget> m_topWidget;
	vec2 m_position;
	vec2 m_size;
	NVGcontextPtr m_nvgContext;
};

} // namespace RED_LILIUM_NAMESPACE
