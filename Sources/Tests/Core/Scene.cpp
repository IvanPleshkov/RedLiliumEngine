#include "pch.h"

#include "gtest/gtest.h"
#include <Ecs/Scene.h>
#include <Ecs/Entity.h>
#include <Ecs/World.h>
#include <Ecs/StandardSystems/HierarchySystem.h>
#include <Ecs/StandardSystems/TransformSystem.h>
#include <Ecs/StandardSystems/RenderSystem.h>

using namespace RED_LILIUM_NAMESPACE;

TEST(Scene, Add1)
{
	Scene scene;
	Entity e1 = scene.Add();
	Entity e2 = scene.Add();
	Entity e3 = scene.Add();

	ASSERT_TRUE(scene.Exists(e1));
	ASSERT_TRUE(scene.Exists(e2));
	ASSERT_TRUE(scene.Exists(e3));
}

TEST(Scene, Add2)
{
	Scene scene;
	Entity e1 = scene.Add();
	Entity e2 = scene.Add();
	scene.Remove(e2);
	scene.Remove(e1);

	Entity e3 = scene.Add();

	ASSERT_TRUE(!scene.Exists(e1));
	ASSERT_TRUE(!scene.Exists(e2));
	ASSERT_TRUE(scene.Exists(e3));
}

TEST(Scene, Add3)
{
	Scene scene;
	Entity e1 = scene.Add();
	Entity e2 = scene.Add();
	Entity e3 = scene.Add();
	scene.Remove(e1);
	scene.Remove(e2);
	scene.Remove(e3);

	scene.Add(e1);
	scene.Add(e3);

	ASSERT_TRUE(scene.Exists(e1));
	ASSERT_TRUE(!scene.Exists(e2));
	ASSERT_TRUE(scene.Exists(e3));
}

TEST(Entity, AddChildren)
{
	// uptr<Scene> scene = umake<Scene>();
	// ptr<Entity> root = scene->Root();
	// root->AddChild<Entity>();
	// root->AddChild<Entity>();

	// auto j = Serialize(scene.get());
}
