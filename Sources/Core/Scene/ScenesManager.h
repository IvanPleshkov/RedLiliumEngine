#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Scene;
class Entity;
class Component;

class ScenesManager : public RedLiliumObject
{
public:
	ScenesManager();
	~ScenesManager() override = default;

};

}  // namespace RED_LILIUM_NAMESPACE
