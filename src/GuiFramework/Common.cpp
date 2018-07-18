#include "pch.h"
#include "Common.h"

using namespace RED_LILIUM_NAMESPACE;

const std::array<Key, AllKeysCount>& RED_LILIUM_NAMESPACE::GetAllKeys()
{
	static std::array<Key, AllKeysCount> allKeys =
	{
		Key::Ctrl,
		Key::Alt,
		Key::Shift,
		Key::Esc,
		Key::Return,
		Key::Tab,
		Key::Space,
		Key::Backspace,
		Key::Up,
		Key::Down,
		Key::Left,
		Key::Right,
		Key::Insert,
		Key::Delete,
		Key::Home,
		Key::End,
		Key::PageUp,
		Key::PageDown,
		Key::Print,
		Key::Plus,
		Key::Minus,
		Key::LeftBracket,
		Key::RightBracket,
		Key::Semicolon,
		Key::Quote,
		Key::Comma,
		Key::Period,
		Key::Slash,
		Key::Backslash,
		Key::Tilde,
		Key::F1,
		Key::F2,
		Key::F3,
		Key::F4,
		Key::F5,
		Key::F6,
		Key::F7,
		Key::F8,
		Key::F9,
		Key::F10,
		Key::F11,
		Key::F12,
		Key::NumPad0,
		Key::NumPad1,
		Key::NumPad2,
		Key::NumPad3,
		Key::NumPad4,
		Key::NumPad5,
		Key::NumPad6,
		Key::NumPad7,
		Key::NumPad8,
		Key::NumPad9,
		Key::Key0,
		Key::Key1,
		Key::Key2,
		Key::Key3,
		Key::Key4,
		Key::Key5,
		Key::Key6,
		Key::Key7,
		Key::Key8,
		Key::Key9,
		Key::KeyA,
		Key::KeyB,
		Key::KeyC,
		Key::KeyD,
		Key::KeyE,
		Key::KeyF,
		Key::KeyG,
		Key::KeyH,
		Key::KeyI,
		Key::KeyJ,
		Key::KeyK,
		Key::KeyL,
		Key::KeyM,
		Key::KeyN,
		Key::KeyO,
		Key::KeyP,
		Key::KeyQ,
		Key::KeyR,
		Key::KeyS,
		Key::KeyT,
		Key::KeyU,
		Key::KeyV,
		Key::KeyW,
		Key::KeyX,
		Key::KeyY,
		Key::KeyZ,
		Key::GamepadA,
		Key::GamepadB,
		Key::GamepadX,
		Key::GamepadY,
		Key::GamepadThumbL,
		Key::GamepadThumbR,
		Key::GamepadShoulderL,
		Key::GamepadShoulderR,
		Key::GamepadUp,
		Key::GamepadDown,
		Key::GamepadLeft,
		Key::GamepadRight,
		Key::GamepadBack,
		Key::GamepadStart,
		Key::GamepadGuide
	};
	return allKeys;
}

