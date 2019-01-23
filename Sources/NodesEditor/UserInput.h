#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>

namespace RED_LILIUM_NAMESPACE
{

enum MouseKey : u8
{
	Left			= 1 << 0,
	Right			= 1 << 1,
	Middle			= 1 << 2,
};
using MouseKeyFlags = Flags<MouseKey>;

struct MouseState
{
	vec2 position = { 0, 0 };
	MouseKeyFlags pressedKeys = MouseKeyFlags();
};

} // namespace RED_LILIUM_NAMESPACE
