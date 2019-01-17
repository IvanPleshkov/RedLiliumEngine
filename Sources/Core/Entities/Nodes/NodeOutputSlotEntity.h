#pragma once

#include <Core/Scene/Entity.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeOutputSlotEntity : public Entity
{
public:
	RED_LILIUM_CLASS(NodeOutputSlotEntity, Entity);

	NodeOutputSlotEntity(ptr<Entity> parent);
	~NodeOutputSlotEntity() override = default;

private:
};

}  // namespace RED_LILIUM_NAMESPACE
