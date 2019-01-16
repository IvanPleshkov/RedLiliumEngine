#include "pch.h"
#include "Scene.h"
#include "Entity.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{
	m_root = umake<Entity>(this);
}

ptr<Entity> Scene::Root()
{
	return m_root.get();
}
