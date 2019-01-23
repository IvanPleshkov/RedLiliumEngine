#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"

using namespace RED_LILIUM_NAMESPACE;

Component::Component()
	: Serializable()
	, m_parent(nullptr)
{}

Component::Component(ptr<Entity> parent)
	: Serializable()
	, m_parent(parent)
{}

void Component::Serialize(ptr<Serializator> serializator) const
{}

void Component::Deserialize(ptr<const Serializator> serializator)
{}
