#pragma once

#include "core/common.h"
#include "nanovg/nanovg.h"

namespace RED_LILIUM_NAMESPACE
{

class Widget;
class Panel;
class GuiManager;
class INativeWindow;
class INativeEnvironment;

using NVGcontextPtr = NVGcontext*;

const float guiFloatEpsilon = 0.01f;

using FontFamily = int;
using Time = double;

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
	None			= 0,
	Left			= 1 << 0,
	Right			= 1 << 1,
	Middle			= 1 << 2
};

enum class MouseEventType
{
	None				= 0,
	MouseMove			= 1 << 0,
	LeftPressed			= 1 << 1,
	LeftReleased		= 1 << 2,
	RightPressed		= 1 << 3,
	RightReleased		= 1 << 4,
	MiddlePressed		= 1 << 5,
	MiddleReleased		= 1 << 6,
	ScrollUp			= 1 << 7,
	ScrollDown			= 1 << 8,
	Dragging			= 1 << 9,
	Dropping			= 1 << 10
};

enum class KeyEventType
{
	None			= 0,
	KeyPressed		= 1 << 0,
	KeyReleased		= 1 << 1
};

struct KeyState
{
};

struct MouseState
{
	MouseState()
		: pressedKeys()
		, mousePosition({ 0, 0 })
		, hoveredWindow(nullptr)
	{}

	ptr<INativeWindow> hoveredWindow;
	vec2 mousePosition;
	float scrollPosition;
	Flags<MouseKey> pressedKeys;
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

enum class GuiPipelineStep : u32
{
	Idle = 1 << 0,
	AddingsWidgets = 1 << 1,
	TickWidgets = 1 << 2,
	SetWidgetsMinimumSize = 1 << 3,
	SetWidgetsDesiredSize = 1 << 4,
	SetWidgetsTransform = 1 << 5,
	Drawing = 1 << 6,
	HandlingMouseEvent = 1 << 7,
	HandlingKeyEvent = 1 << 8,
	PostTickWidgets = 1 << 9,
};

} // namespace RED_LILIUM_NAMESPACE
