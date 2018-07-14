#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Widget : public RedLiliumObject
{
public:
	~Widget() override;

	template <class TWidget>
	ptr<TWidget> Add();

	void SetPosition(vec2 position);
	void SetSize(vec2 size);

	vec2 GetPosition() const;
	vec2 GetSize() const;
	vec2 GetMinimumSize() const;
	vec2 GetDesiredSize() const;

	bool IsFocused(bool includeChildren = false) const;
	bool IsHovered(bool includeChildren = false) const;
	
	ptr<Panel> GetPanel();
	ptr<GuiManager> GetGuiManager();
	ptr<const Panel> GetPanel() const;
	ptr<const GuiManager> GetGuiManager() const;
	const MouseState& GetMouseState() const;
	const KeyState& GetKeyState() const;

	ptr<Style> GetStyle();

public:
	virtual bool CanFocusing() const;

protected:
	virtual bool HandleKeyEvent(const KeyEvent& keyEvent);
	virtual bool HandleMouseEvent(const MouseEvent& mouseEvent);
	virtual bool IsContainingPoint(vec2 point);

	virtual void Update();
	virtual void CreateFirstChilds();
	virtual void CreateLastChilds();

	virtual void UpdateDesiredSize();

	virtual void UpdateMinimumSize();

	virtual void Layout();

	virtual void Draw();
	virtual void PostDraw();

	virtual void OnFocus();
	virtual void OnFocusLost();

	virtual void OnHover();
	virtual void OnHoverLost();

protected:
	friend class Panel;
	friend class GuiManager;

	NVGcontextPtr GetNvgContext();
	void BeginChildAdding();
	void EndChildAdding();

	std::type_index GetWidgetTypeIndex() const;

	template <class TWidget>
	bool IsAddingChildExists();

	template <class TWidget>
	ptr<TWidget> AddNewChild();

	ptr<Panel> m_panel = nullptr;
	ptr<Widget> m_parent = nullptr;
	std::vector<uptr<Widget>> m_children;
	u32 m_childAddingCursor = 0;

	vec2 m_position;
	vec2 m_size;
	vec2 m_desiredSize;
	vec2 m_minimumSize;

	bool m_visible = true;
	bool m_enable = true;
};

}  // namespace RED_LILIUM_NAMESPACE

#include "Widget.hpp"
