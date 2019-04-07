#pragma once

/*

#include <Core/Common.h>
#include <Reflection/Serialization.h>
#include "Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity;

class Scene final : public Serializable
{
public:
	RED_LILIUM_CLASS(Scene, Serializable);

	Scene();
	~Scene() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	ptr<Entity> GetRoot();
	ptr<const Entity> GetRoot() const;

private:
	uptr<Entity> m_root;
};

} // namespace RED_LILIUM_NAMESPACE

*/
