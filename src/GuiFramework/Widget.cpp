#include "pch.h"
#include "Widget.h"
#include "Panel.h"
#include "GuiManager.h"

using namespace RED_LILIUM_NAMESPACE;

Widget::~Widget()
{
	if (IsHovered())
	{
		GetGuiManager()->SetHoveredWidget(nullptr);
	}

	if (IsFocused())
	{
		GetGuiManager()->SetFocusedWidget(nullptr);
	}
}

void Widget::SetSize(vec2 size)
{
	RED_LILIUM_ASSERT(m_panel->GetCurrentPipelineStep() == GuiPipelineStep::SetWidgetsTransform);

	m_size = size;
	m_size.x = std::max(m_size.x, m_minimumSize.x);
	m_size.y = std::max(m_size.y, m_minimumSize.y);
}

void Widget::SetTransform(mat3 transform)
{
	RED_LILIUM_ASSERT(m_panel->GetCurrentPipelineStep() == GuiPipelineStep::SetWidgetsTransform);

	m_transform = transform;
}

const mat3& Widget::GetTransform() const
{
	return m_transform;
}

mat3 Widget::GetInvertedTransform() const
{
	return glm::inverse(m_transform);
}

void Widget::SetPosition(vec2 position)
{
	RED_LILIUM_ASSERT(m_panel->GetCurrentPipelineStep() == GuiPipelineStep::SetWidgetsTransform);

	m_transform = glm::translate(mat3(1.0f), position);
}

vec2 Widget::GetPosition() const
{
	return { m_transform[2][0], m_transform[2][1] };
}

vec2 Widget::GetSize() const
{
	return m_size;
}

vec2 Widget::GetMinimumSize() const
{
	return m_minimumSize;
}

vec2 Widget::GetDesiredSize() const
{
	return m_desiredSize;
}

bool Widget::CanFocusing() const
{
	return m_canFocusing;
}

bool Widget::CanHovering() const
{
	return m_canHovering;
}

void Widget::SetFocusing(bool value)
{
	m_canFocusing = value;
}

void Widget::SetHovering(bool value)
{
	m_canHovering = value;
}

bool Widget::IsFocused() const
{
	return m_panel->GetGuiManager()->m_focusedWidget == this;
}

bool Widget::HasFocusedChild() const
{
	for (auto& child : m_children)
	{
		if (child->IsFocused() || child->HasFocusedChild())
		{
			return true;
		}
	}

	return false;
}

bool Widget::IsHovered() const
{
	return m_panel->GetGuiManager()->m_hoveredWidget == this;
}

bool Widget::HasHoveredChild() const
{
	for (auto& child : m_children)
	{
		if (child->IsHovered() || child->HasHoveredChild())
		{
			return true;
		}
	}

	return false;
}

const MouseState& Widget::GetMouseState() const
{
	return GetGuiManager()->GetMouseState();
}

const KeyState& Widget::GetKeyState() const
{
	return GetGuiManager()->GetKeyState();
}

NVGcontextPtr Widget::GetNvgContext()
{
	return m_panel->GetNvgContext();
}

ptr<Panel> Widget::GetPanel()
{
	return m_panel;
}
ptr<const Panel> Widget::GetPanel() const
{
	return m_panel;
}

ptr<GuiManager> Widget::GetGuiManager()
{
	return m_panel->GetGuiManager();
}

ptr<const GuiManager> Widget::GetGuiManager() const
{
	return m_panel->GetGuiManager();
}

ptr<Style> Widget::GetStyle()
{
	return m_panel->GetGuiManager()->GetStyle();
}

std::type_index Widget::GetWidgetTypeIndex() const
{
	return std::type_index(typeid(*this));
}

bool Widget::HandleKeyEvent(const KeyEvent& keyEvent)
{
	return false;
}

bool Widget::HandleMouseEvent(const MouseEvent& mouseEvent)
{
	return false;
}

bool Widget::IsContainingPoint(vec2 point)
{
	return true;
}

void Widget::Tick() { }

void Widget::PostTick() { }

void Widget::CreateFirstChilds() { }

void Widget::CreateLastChilds() { }

void Widget::UpdateDesiredSize()
{
	m_desiredSize = m_minimumSize;

	for (auto& child : m_children)
	{
		m_desiredSize.x = std::max(m_desiredSize.x, child->m_desiredSize.x);
		m_desiredSize.y = std::max(m_desiredSize.y, child->m_desiredSize.y);
	}
}

void Widget::UpdateMinimumSize()
{
	m_minimumSize = { 0, 0 };

	for (auto& child : m_children)
	{
		m_minimumSize.x = std::max(m_minimumSize.x, child->m_minimumSize.x);
		m_minimumSize.y = std::max(m_minimumSize.y, child->m_minimumSize.y);
	}
}

void Widget::Layout()
{
	for (auto& child : m_children)
	{
		child->SetTransform(mat3(1.0f));
		child->SetSize(m_size);
	}
}

void Widget::Draw() {}

void Widget::PostDraw() {}

void Widget::OnFocus() {}

void Widget::OnFocusLost() {}

void Widget::OnHover() {}

void Widget::OnHoverLost() {}

void Widget::BeginChildAdding()
{
	m_childAddingCursor = 0;
	CreateFirstChilds();
}

void Widget::EndChildAdding()
{
	CreateLastChilds();

	// remove obsolete childs
	m_children.erase(m_children.begin() + m_childAddingCursor, m_children.end());
	m_childAddingCursor = 0;

	for (auto& child : m_children)
	{
		child->EndChildAdding();
	}
}
