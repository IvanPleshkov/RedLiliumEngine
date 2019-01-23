#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{
	m_root = umake<Entity>(this);
}

void Scene::Serialize(ptr<Serializator> serializator) const
{}

void Scene::Deserialize(ptr<const Serializator> serializator)
{}

ptr<Entity> Scene::GetRoot()
{
	return m_root.get();
}

ptr<const Entity> Scene::GetRoot() const
{
	return m_root.get();
}
