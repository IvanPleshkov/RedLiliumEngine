#pragma once

#include "Common.h"
#include "Restorable.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity;

class Scene final : public Restorable
{
public:
	Scene();
	~Scene() override = default;

	ptr<Entity> Root();

private:
	uptr<Entity> m_root;
};

}  // namespace RED_LILIUM_NAMESPACE
