#pragma once

#include "Common.h"
#include "Restorable.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity;
class Component;

class Scene final : public Restorable
{
public:
	Scene();
	~Scene() override = default;

	rptr<Entity> Root();

private:
	u64 m_maxId = 0;
	std::unordered_map<u64, uptr<Entity>> m_entities;
};

}  // namespace RED_LILIUM_NAMESPACE
