#include "pch.h"

#include "gtest/gtest.h"
#include <Core/Scene/Scene.h>
#include <Core/Scene/Entity.h>
#include <Core/Entities/Nodes/NodeEntity.h>
#include <Core/Entities/Nodes/NodeInputSlotEntity.h>
#include <Core/Entities/Nodes/NodeOutputSlotEntity.h>

using namespace RED_LILIUM_NAMESPACE;

TEST(Scene, Constructor)
{
	uptr<Scene> scene = umake<Scene>();
}

TEST(Entity, AddChildren)
{
	uptr<Scene> scene = umake<Scene>();
	ptr<Entity> root = scene->Root();
	root->AddChild<Entity>();
	root->AddChild<Entity>();

	auto j = Serialize(scene.get());
}
