#pragma once

#include <Core/Scene/Entity.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeInputSlotEntity : public Entity
{
public:
	RED_LILIUM_CLASS(NodeInputSlotEntity, Entity);

	NodeInputSlotEntity(ptr<Entity> parent);
	~NodeInputSlotEntity() override = default;

private:
};

}  // namespace RED_LILIUM_NAMESPACE
