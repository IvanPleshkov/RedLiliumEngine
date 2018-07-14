#include "pch.h"
#include "Panel.h"
#include "Widget.h"

using namespace RED_LILIUM_NAMESPACE;

Panel::Panel(const ptr<GuiManager>& guiManager)
	: m_guiManager(guiManager)
{
	m_currentGuiPipelineState = GuiPipelineStep::Idle;
	m_topWidget = std::make_unique<Widget>();
	m_topWidget->m_panel = this;
}

ptr<Widget> Panel::BeginFrame()
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::Idle);
	m_currentGuiPipelineState = GuiPipelineStep::AddingsWidgets;

	m_topWidget->BeginChildAdding();
	return m_topWidget.get();
}

void Panel::EndFrame(NVGcontextPtr nvgContext, vec2 position, vec2 size)
{
	RED_LILIUM_ASSERT(m_currentGuiPipelineState == GuiPipelineStep::AddingsWidgets);

	m_nvgContext = nvgContext;
	m_position = position;
	m_size = size;

	m_topWidget->EndChildAdding();

	m_currentGuiPipelineState = GuiPipelineStep::TickWidgets;
	UpdateWidgets(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsMinimumSize;
	UpdateWidgetsMinimumSize(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsDesiredSize;
	UpdateWidgetsDesiredSize(m_topWidget);

	m_topWidget->m_position = position;
	m_topWidget->m_size = size;
	m_topWidget->m_size.x = std::max(m_topWidget->m_size.x, m_topWidget->m_minimumSize.x);
	m_topWidget->m_size.y = std::max(m_topWidget->m_size.y, m_topWidget->m_minimumSize.y);

	m_currentGuiPipelineState = GuiPipelineStep::SetWidgetsTransform;
	UpdateWidgetsLayouting(m_topWidget);

	m_currentGuiPipelineState = GuiPipelineStep::Idle;
}

NVGcontextPtr Panel::GetNvgContext()
{
	return m_nvgContext;
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

	DrawRecursive(m_topWidget.get());
	nvgResetScissor(m_nvgContext);

	m_currentGuiPipelineState = GuiPipelineStep::Idle;
}

ptr<Widget> Panel::GetWidgetUnderPoint(vec2 position)
{
	return GetWidgetUnderPoint(m_topWidget, position);
}

void Panel::DrawRecursive(const ptr<Widget>& widget)
{
	SetWidgetScissor(widget);
	widget->Draw();

	for (auto& child : widget->m_children)
	{
		DrawRecursive(child.get());
	}

	SetWidgetScissor(widget);
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

void Panel::UpdateWidgets(const uptr<Widget>& widget)
{
	for (const uptr<Widget>& child : widget->m_children)
	{
		UpdateWidgets(child);
	}

	widget->Update();
}

ptr<Widget> Panel::GetWidgetUnderPoint(const uptr<Widget>& widget, vec2 position)
{
	if (widget->m_position.x > position.x ||
		widget->m_position.y > position.y ||
		widget->m_position.x + widget->m_size.x < position.x ||
		widget->m_position.y + widget->m_size.y < position.y)
	{
		return nullptr;
	}

	if (!widget->m_visible || !widget->IsContainingPoint(position))
	{
		return nullptr;
	}

	for (auto i = widget->m_children.rbegin(); i != widget->m_children.rend(); i++)
	{
		ptr<Widget> result = GetWidgetUnderPoint(*i, position);
		if (result != nullptr)
		{
			return result;
		}
	}

	return widget.get();
}

void Panel::SetWidgetScissor(const ptr<Widget>& widget) const
{
	nvgScissor(m_nvgContext,
		m_position.x,
		m_position.y,
		m_size.x,
		m_size.y);

	nvgIntersectScissor(m_nvgContext,
		widget->GetPosition().x,
		widget->GetPosition().y,
		widget->GetSize().x,
		widget->GetSize().y);
}
