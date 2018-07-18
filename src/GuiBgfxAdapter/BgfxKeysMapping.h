#pragma once

#include <entry/entry.h> // bgfx
#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{

std::optional<Key> convertBgfxKey(entry::Key::Enum key);
std::optional<entry::Key::Enum> convertBgfxKey(Key key);

} // namespace RED_LILIUM_NAMESPACE
