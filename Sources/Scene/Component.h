#pragma once

#include <Core/Common.h>
#include <Reflection/Serialization.h>

namespace RED_LILIUM_NAMESPACE
{

class Scene;
class Entity;

class Component : public Serializable
{
public:
	RED_LILIUM_CLASS(Component, Serializable);

	Component();
	Component(ptr<Entity> parent);
	~Component() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;
		
private:
	ptr<Entity> m_parent;
};

} // namespace RED_LILIUM_NAMESPACE
