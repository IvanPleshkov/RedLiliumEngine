#pragma once

#include <Core/Common.h>
#include <Core/FileSystem.h>
#include <Scene/Entity.h>

namespace RED_LILIUM_NAMESPACE
{

bool LoadSceneByAssimp(ptr<FileSystem> filesystem, std::string_view filename, ptr<Entity> rootEntity);

} // namespace RED_LILIUM_NAMESPACE
