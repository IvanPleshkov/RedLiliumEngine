#pragma once

#include <Core/Common.h>
#include <Core/Reflection/Serialization.h>

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

	ptr<Entity> Root();

private:
	uptr<Entity> m_root;
};

}  // namespace RED_LILIUM_NAMESPACE
