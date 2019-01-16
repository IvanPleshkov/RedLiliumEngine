#pragma once

#include "Scene/Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class NodeOutputSlotEntity : public Entity
{
public:
	NodeOutputSlotEntity(ptr<Entity> parent);
	~NodeOutputSlotEntity() override = default;

private:
};

}  // namespace RED_LILIUM_NAMESPACE
