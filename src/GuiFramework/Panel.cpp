#include "pch.h"
#include "Panel.h"
#include "Widget.h"
#include "INativeWindow.h"

using namespace RED_LILIUM_NAMESPACE;

Panel::Panel(const ptr<GuiManager>& guiManager)
	: m_guiManager(guiManager)
	, m_nativeWindow(nullptr)
{
	m_currentGuiPipelineState = GuiPipelineStep::Idle;
	m_topWidget = std::make_unique<Widget>();
	m_topWidget->m_panel = this;
}

void Panel::BeginFrame()
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::Idle);

	m_currentGuiPipelineState = GuiPipelineStep::ConfigureWidgets;
	m_topWidget->BeginChildAdding();
}

void Panel::EndFrame()
{
	RED_LILIUM_ASSERT(m_nativeWindow);
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::ConfigureWidgets);

	m_topWidget->EndChildAdding();

	m_currentGuiPipelineState = GuiPipelineStep::TickWidgets;
	TickWidgetsRecursive(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsMinimumSize;
	UpdateWidgetsMinimumSize(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsDesiredSize;
	UpdateWidgetsDesiredSize(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsTransform;

	m_topWidget->SetTransform(mat3(1.0f));
	m_topWidget->SetSize(m_size);

	UpdateWidgetsLayouting(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::PostTickWidgets;
	PostTickWidgetsRecursive(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::Idle;
}

ptr<Widget> Panel::GetTopWidget()
{
	return m_topWidget.get();
}

ptr<const Widget> Panel::GetTopWidget() const
{
	return m_topWidget.get();
}

void Panel::SetWindow(ptr<INativeWindow> nativeWindow)
{
	// TODO: what state of SetWindow ?
	// RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::AddingsWidgets);

	m_nativeWindow = nativeWindow;
}

ptr<INativeWindow> Panel::GetWindow()
{
	return m_nativeWindow;
}

ptr<const INativeWindow> Panel::GetWindow() const
{
	return m_nativeWindow;
}

void Panel::SetSize(vec2 size)
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::ConfigureWidgets);

	m_size = size;
}

void Panel::SetPosition(vec2 position)
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::ConfigureWidgets);

	m_position = position;
}

vec2 Panel::GetSize() const
{
	return m_size;
}

vec2 Panel::GetPosition() const
{
	return m_position;
}

NVGcontextPtr Panel::GetNvgContext()
{
	RED_LILIUM_ASSERT(m_nativeWindow);
	RED_LILIUM_ASSERT(m_nativeWindow->GetNvgContext());

	return m_nativeWindow->GetNvgContext();
}

ptr<GuiManager> Panel::GetGuiManager()
{
	return m_guiManager;
}

ptr<const GuiManager> Panel::GetGuiManager() const
{
	return m_guiManager;
}

GuiPipelineStep Panel::GetCurrentPipelineStep() const
{
	return m_currentGuiPipelineState;
}

void Panel::Draw()
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::Idle);
	m_currentGuiPipelineState = GuiPipelineStep::Drawing;

	nvgResetScissor(GetNvgContext());
	DrawRecursive(m_topWidget.get(), mat3(1.0f));
	nvgResetScissor(GetNvgContext());

	m_currentGuiPipelineState = GuiPipelineStep::Idle;
}

ptr<Widget> Panel::GetWidgetUnderPoint(vec2 position, bool onlyHoverableWidgets)
{
	return GetWidgetUnderPointRecursive(m_topWidget, position, onlyHoverableWidgets);
}

void Panel::DrawRecursive(const ptr<Widget>& widget, mat3 transform)
{
	mat3 widgetTransform = transform * widget->GetTransform();

	SetWidgetScissor(widget, widgetTransform);
	widget->Draw();

	for (auto& child : widget->m_children)
	{
		DrawRecursive(child.get(), widgetTransform);
	}

	SetWidgetScissor(widget, widgetTransform);
	widget->PostDraw();
}

void Panel::UpdateWidgetsDesiredSize(const uptr<Widget>& widget)
{
	for (const uptr<Widget>& child : widget->m_children)
	{
		UpdateWidgetsDesiredSize(child);
	}

	widget->UpdateDesiredSize();

	RED_LILIUM_ASSERT(widget->m_desiredSize.x > widget->m_minimumSize.x - guiFloatEpsilon);
	RED_LILIUM_ASSERT(widget->m_desiredSize.y > widget->m_minimumSize.y - guiFloatEpsilon);
}

void Panel::UpdateWidgetsMinimumSize(const uptr<Widget>& widget)
{
	for (const uptr<Widget>& child : widget->m_children)
	{
		UpdateWidgetsMinimumSize(child);
	}

	widget->UpdateMinimumSize();
}

void Panel::UpdateWidgetsLayouting(const uptr<Widget>& widget)
{
	RED_LILIUM_ASSERT(widget->m_size.x > widget->m_minimumSize.x - guiFloatEpsilon);
	RED_LILIUM_ASSERT(widget->m_size.y > widget->m_minimumSize.y - guiFloatEpsilon);

	widget->Layout();

	for (const uptr<Widget>& child : widget->m_children)
	{
		UpdateWidgetsLayouting(child);
	}
}

void Panel::TickWidgetsRecursive(const uptr<Widget>& widget)
{
	for (const uptr<Widget>& child : widget->m_children)
	{
		TickWidgetsRecursive(child);
	}

	widget->Tick();
}

void Panel::PostTickWidgetsRecursive(const uptr<Widget>& widget)
{
	for (const uptr<Widget>& child : widget->m_children)
	{
		PostTickWidgetsRecursive(child);
	}

	widget->PostTick();
}

ptr<Widget> Panel::GetWidgetUnderPointRecursive(const uptr<Widget>& widget, vec2 position, bool onlyHoverableWidgets)
{
	vec2 localPosition = widget->GetInvertedTransform() * vec3(position, 1.0f);

	if (localPosition.x < 0 ||
		localPosition.y < 0 ||
		localPosition.x > widget->GetSize().x ||
		localPosition.y > widget->GetSize().y)
	{
		return nullptr;
	}

	if (!widget->m_visible || !widget->IsContainingPoint(localPosition))
	{
		return nullptr;
	}

	for (auto i = widget->m_children.rbegin(); i != widget->m_children.rend(); i++)
	{
		ptr<Widget> result = GetWidgetUnderPointRecursive(*i, localPosition, onlyHoverableWidgets);
		if (result != nullptr)
		{
			return result;
		}
	}

	const bool canHovered = !onlyHoverableWidgets || widget->CanHovering();
	if (canHovered)
	{
		return widget.get();
	}
	else
	{
		return nullptr;
	}
}

void Panel::SetWidgetScissor(const ptr<Widget>& widget, mat3 transform)
{
	nvgResetTransform(GetNvgContext());

	auto transposed = glm::transpose(transform);
	auto row0 = transposed[0];
	auto row1 = transposed[1];
	nvgTransform(GetNvgContext(), 
		row0[0], row1[0],
		row0[1], row1[1],
		row0[2], row1[2]);

	nvgScissor(
		GetNvgContext(), 0, 0,
		widget->GetSize().x,
		widget->GetSize().y);
}
