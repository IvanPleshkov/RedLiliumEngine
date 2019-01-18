#include "pch.h"

#include "gtest/gtest.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>

using namespace RED_LILIUM_NAMESPACE;

TEST(Scene, Constructor)
{
	uptr<Scene> scene = umake<Scene>();
}

TEST(Entity, AddChildren)
{
	// uptr<Scene> scene = umake<Scene>();
	// ptr<Entity> root = scene->Root();
	// root->AddChild<Entity>();
	// root->AddChild<Entity>();

	// auto j = Serialize(scene.get());
}
