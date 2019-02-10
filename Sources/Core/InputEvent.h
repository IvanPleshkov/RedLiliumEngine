#pragma once

#include "Common.h"
#include "Flags.h"

namespace RED_LILIUM_NAMESPACE
{

enum class KeyModifier : u8
{
	LeftCtrl			= 1 << 0,
	RightCtrl			= 1 << 1,
	LeftAlt				= 1 << 2,
	RightAlt			= 1 << 3,
	LeftShift			= 1 << 4,
	RightShift			= 1 << 5,
	LeftMeta			= 1 << 6,
	RightMeta			= 1 << 7,

	Ctrl				= LeftCtrl		| RightCtrl,
	Alt					= LeftAlt		| RightAlt,
	Shift				= LeftShift		| RightShift,
	Meta				= LeftMeta		| RightMeta,
};
using KeyModifierFlags = Flags<KeyModifier>;

enum class MouseKey : u8
{
	Left				= 1 << 0,
	Right				= 1 << 1,
	Middle				= 1 << 2,
};
using MouseKeyFlags = Flags<MouseKey>;

enum class InputType : u8
{
	Pressed				= 1 << 0,
	Released			= 1 << 1,
	Move				= 1 << 2,
	WheelUp				= 1 << 3,
	WheelDown			= 1 << 4,
};
using InputTypeFlags = Flags<InputType>;

enum class Key : u8
{
	LeftCtrl,
	RightCtrl,
	LeftAlt,
	RightAlt,
	LeftShift,
	RightShift,
	LeftMeta,
	RightMeta,
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
	GamepadGuide,
};

struct InputState
{
	std::vector<Key> pressedKeys;
	MouseKeyFlags pressedMouseKeys;
	vec2 mousePosition;
};

struct MouseEvent
{
	InputType inputType;
	MouseKeyFlags mouseKeys;
	vec2 mousePosition;
	KeyModifierFlags keyModifiers;
};

} // RED_LILIUM_NAMESPACE
