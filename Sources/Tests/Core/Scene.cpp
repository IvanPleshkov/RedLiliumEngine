#include "pch.h"

#include <catch2/catch.hpp>

#include <Ecs/Scene.h>
#include <Ecs/Entity.h>
#include <Ecs/World.h>
#include <Ecs/StandardSystems/HierarchySystem.h>
#include <Ecs/StandardSystems/TransformSystem.h>
#include <Ecs/StandardSystems/RenderSystem.h>

using namespace RED_LILIUM_NAMESPACE;

TEST_CASE("Ecs.Test1")
{
	Scene scene;
	Entity e1 = scene.Add();
	Entity e2 = scene.Add();
	Entity e3 = scene.Add();

	REQUIRE(scene.Exists(e1));
	REQUIRE(scene.Exists(e2));
	REQUIRE(scene.Exists(e3));
}

TEST_CASE("Ecs.Test2")
{
	Scene scene;
	Entity e1 = scene.Add();
	Entity e2 = scene.Add();
	scene.Remove(e2);
	scene.Remove(e1);

	Entity e3 = scene.Add();

	REQUIRE(!scene.Exists(e1));
	REQUIRE(!scene.Exists(e2));
	REQUIRE(scene.Exists(e3));
}

TEST_CASE("Ecs.Test3")
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

	REQUIRE(scene.Exists(e1));
	REQUIRE(!scene.Exists(e2));
	REQUIRE(scene.Exists(e3));
}

TEST_CASE("Ecs.Test4")
{
	Scene scene;
	Entity e1 = scene.Add();

	scene.AddComponent<HierarchyComponent>(e1);

	REQUIRE(scene.HasComponent<HierarchyComponent>(e1));
	REQUIRE(!scene.HasComponent<TransformComponent>(e1));
}

TEST_CASE("Ecs.Test5")
{
	Scene scene;
	Entity e1 = scene.Add();

	scene.AddComponent<HierarchyComponent>(e1);
	scene.RemoveComponent<HierarchyComponent>(e1);

	REQUIRE(!scene.HasComponent<HierarchyComponent>(e1));
	REQUIRE(!scene.HasComponent<TransformComponent>(e1));
}

TEST_CASE("Ecs.Test6")
{
	Scene scene;
	Entity e1 = scene.Add();

	scene.AddComponent<HierarchyComponent>(e1);
	scene.Remove(e1);

	REQUIRE(!scene.Exists(e1));
}

TEST_CASE("Ecs.Test7")
{
	Scene scene;
	Entity e1 = scene.Add();

	scene.AddComponent<HierarchyComponent>(e1);
	auto hierarchyComponent = scene.GetComponent<HierarchyComponent>(e1);

	REQUIRE(hierarchyComponent != nullptr);
}

TEST_CASE("Ecs.Test8")
{
	Scene scene;
	Entity e1 = scene.Add();

	scene.AddComponent<HierarchyComponent>(e1);
	const auto hierarchyComponent = scene.GetComponent<HierarchyComponent>(e1);

	REQUIRE(hierarchyComponent != nullptr);
}
