#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Panel : public RedLiliumObject
{
public:
	friend class GuiManager;

	Panel(ptr<GuiManager> guiManager);
	~Panel() override {}

public:
	ptr<Widget> GetTopWidget();
	ptr<const Widget> GetTopWidget() const;

	void BeginFrame();
	void EndFrame();

	void SetWindow(ptr<INativeWindow> nativeWindow);
	ptr<INativeWindow> GetWindow();
	ptr<const INativeWindow> GetWindow() const;

	void SetSize(vec2 size);
	vec2 GetSize() const;

	void SetPosition(vec2 position);
	vec2 GetPosition() const;

	NVGcontextPtr GetNvgContext();
	ptr<GuiManager> GetGuiManager();
	ptr<const GuiManager> GetGuiManager() const;
	ptr<Widget> GetWidgetUnderPoint(vec2 position, bool onlyHoverableWidgets = true);
	GuiPipelineStep GetCurrentPipelineStep() const;

	void Draw();

private:
	void DrawRecursive(ptr<Widget> widget, mat3 transform);
	void TickWidgetsRecursive(const uptr<Widget>& widget);
	void PostTickWidgetsRecursive(const uptr<Widget>& widget);

	void UpdateWidgetsDesiredSize(const uptr<Widget>& widget);
	void UpdateWidgetsMinimumSize(const uptr<Widget>& widget);
	void UpdateWidgetsLayouting(const uptr<Widget>& widget);

	ptr<Widget> GetWidgetUnderPointRecursive(
		const uptr<Widget>& widget,
		vec2 position,
		bool onlyHoverableWidgets);
	void SetWidgetScissor(ptr<Widget> widget, mat3 transform);

private:
	GuiPipelineStep m_currentGuiPipelineState;
	ptr<GuiManager> m_guiManager;
	ptr<INativeWindow> m_nativeWindow;
	uptr<Widget> m_topWidget;
	vec2 m_position;
	vec2 m_size;
};

} // namespace RED_LILIUM_NAMESPACE
