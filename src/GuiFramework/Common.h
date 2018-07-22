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
class GuiReplayData;
class GuiRecorder;


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

enum class MouseKey
{
	Left			= 1 << 0,
	Right			= 1 << 1,
	Middle			= 1 << 2
};

enum class MouseEventType
{
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

enum class Key
{
	Ctrl = 0,
	Alt,
	Shift,
	Esc,
	Return,
	Tab,
	Space,
	Backspace,
	Up,
	Down,
	Left,
	Right,
	Insert,
	Delete,
	Home,
	End,
	PageUp,
	PageDown,
	Print,
	Plus,
	Minus,
	LeftBracket,
	RightBracket,
	Semicolon,
	Quote,
	Comma,
	Period,
	Slash,
	Backslash,
	Tilde,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	NumPad0,
	NumPad1,
	NumPad2,
	NumPad3,
	NumPad4,
	NumPad5,
	NumPad6,
	NumPad7,
	NumPad8,
	NumPad9,
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,

	GamepadA,
	GamepadB,
	GamepadX,
	GamepadY,
	GamepadThumbL,
	GamepadThumbR,
	GamepadShoulderL,
	GamepadShoulderR,
	GamepadUp,
	GamepadDown,
	GamepadLeft,
	GamepadRight,
	GamepadBack,
	GamepadStart,
	GamepadGuide
};

const u32 AllKeysCount = 103;
const std::array<Key, AllKeysCount>& GetAllKeys();

enum class KeyModifier
{
	Ctrl	= 1 << 0,
	Alt		= 1 << 1,
	Shift	= 1 << 2
};

struct Shortcut
{
	Shortcut(Key key)
		: key(key)
	{}

	Shortcut(Key key, KeyModifier modifier)
		: key(key)
	{
		this->modifier.Add(modifier);
	}

	Shortcut(Key key, KeyModifier modifier0, KeyModifier modifier1)
		: key(key)
	{
		this->modifier.Add(modifier0);
		this->modifier.Add(modifier1);
	}

	Shortcut(Key key, KeyModifier modifier0, KeyModifier modifier1, KeyModifier modifier2)
		: key(key)
	{
		this->modifier.Add(modifier0);
		this->modifier.Add(modifier1);
		this->modifier.Add(modifier2);
	}

	// TODO:
	std::string GetName() const;

	Flags<KeyModifier> modifier;
	Key key;
};

enum class KeyEventType
{
	None			= 0,
	KeyPressed		= 1 << 0,
	KeyReleased		= 1 << 1
};

struct KeyState
{
	KeyState()
	{
		for (bool& isPressed : pressedKeys)
		{
			isPressed = false;
		}
	}

	bool IsPressed(Key key) const
	{
		return pressedKeys[static_cast<size_t>(key)];
	}

	bool& IsPressed(Key key)
	{
		return pressedKeys[static_cast<size_t>(key)];
	}

	std::array<bool, AllKeysCount> pressedKeys;
};

struct KeyEvent
{
	KeyEventType eventType;
	Key key;
	Flags<KeyModifier> modifiers;
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
	MouseEvent(MouseEventType eventType)
		: eventType(eventType)
	{}

	bool IsMouseKeyPressedEvent() const
	{
		return 
			eventType == MouseEventType::LeftPressed ||
			eventType == MouseEventType::RightPressed ||
			eventType == MouseEventType::MiddlePressed;
	}

	bool IsMouseKeyReleasedEvent() const
	{
		return
			eventType == MouseEventType::LeftReleased ||
			eventType == MouseEventType::RightReleased ||
			eventType == MouseEventType::MiddleReleased;
	}

	std::optional<MouseKey> GetEventedMouseKey() const
	{
		switch (eventType)
		{
		case rl::MouseEventType::LeftPressed:
		case rl::MouseEventType::LeftReleased:
			return MouseKey::Left;

		case rl::MouseEventType::RightPressed:
		case rl::MouseEventType::RightReleased:
			return MouseKey::Right;

		case rl::MouseEventType::MiddlePressed:
		case rl::MouseEventType::MiddleReleased:
			return MouseKey::Middle;
		}
		return std::nullopt;
	}

	MouseEventType eventType;
	Flags<KeyModifier> modifiers;
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
	Idle						= 1 << 0,
	ConfigureWidgets			= 1 << 1,
	TickWidgets					= 1 << 2,
	SetWidgetsMinimumSize		= 1 << 3,
	SetWidgetsDesiredSize		= 1 << 4,
	SetWidgetsTransform			= 1 << 5,
	Drawing						= 1 << 6,
	HandlingMouseEvent			= 1 << 7,
	HandlingKeyEvent			= 1 << 8,
	PostTickWidgets				= 1 << 9,
};

} // namespace RED_LILIUM_NAMESPACE
