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
	u64 m_maxId = 0;
	uptr<Entity> m_root;
};

}  // namespace RED_LILIUM_NAMESPACE
