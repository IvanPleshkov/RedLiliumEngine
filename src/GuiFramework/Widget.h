#pragma once

#include "core/common.h"
#include "nanovg/nanovg.h"

namespace RED_LILIUM_NAMESPACE
{
class Panel;
class GuiManager;

using NVGcontextPtr = NVGcontext*;

const float guiFloatEpsilon = 0.01f;

using FontFamily = int;

enum class HorizontalAlignment : u32
{
	Center		= 1 << 0,
	Left		= 1 << 1,
	Right		= 1 << 2,
	Fill		= 1 << 3
};

enum class VerticalAlignment : u32
{
	Center		= 1 << 0,
	Top			= 1 << 1,
	Bottom		= 1 << 2,
	Fill		= 1 << 3
};

enum class Direction : u32
{
	Horizontal				= 1 << 0,
	Vertical				= 1 << 1,
	RevertedHorizontal		= 1 << 2,
	RevertedVertical		= 1 << 3
};

class KeyEvent
{};

enum class MouseKey
{
	None	= 0,
	Left	= 1 << 0,
	Right	= 1 << 1,
	Middle	= 1 << 2
};

enum class MouseEventType
{
	None			= 0,
	MouseMove		= 1 << 0,
	LeftPressed		= 1 << 1,
	LeftReleased	= 1 << 2,
	RightPressed	= 1 << 3,
	RightReleased	= 1 << 4,
	MiddlePressed	= 1 << 5,
	MiddleReleased	= 1 << 6,
	ScrollUp		= 1 << 7,
	ScrollDown		= 1 << 8,
	Dragging		= 1 << 9,
	Dropping		= 1 << 10
};

enum class KeyEventType
{
	None			= 0,
	KeyPressed		= 1 << 0,
	KeyReleased		= 1 << 1
};

class Window : public RedLiliumObject
{
public:
	~Window() override {}

	u32 viewId;
};

struct KeyState
{
};

struct MouseState
{
	MouseState()
		: pressedKeys()
		, mousePosition({ 0, 0 })
		, droppingObject(nullptr)
		, hoveredWindow(nullptr)
	{}

	ptr<Window> hoveredWindow;
	vec2 mousePosition;
	float scrollPosition;
	Flags<MouseKey> pressedKeys;
	ptr<RedLiliumObject> droppingObject;
};

struct MouseEvent
{
	MouseEvent()
		: eventType(MouseEventType::None)
	{}

	MouseEventType eventType;
};

struct FontSettings
{
	vec4 color = { 1, 1, 1, 1 };
	float fontSize = 14;
	FontFamily fontFamily = -1;
};

class Style : public RedLiliumObject
{
public:
	Style()
	{}

	FontSettings m_h1;
	FontSettings m_h2;
	FontSettings m_regular;
	FontSettings m_disabled;
	FontSettings m_caption;
};

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
