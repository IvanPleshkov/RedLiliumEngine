#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class Component : public RedLiliumObject
{
public:
	Component(ptr<Entity> entity);
	~Component() override = default;

	ptr<Entity> GetEntity();
	ptr<const Entity> GetEntity() const;

	virtual std::string GetName() = 0;
	virtual void DumpToJson(json& j) = 0;

private:
	ptr<Entity> m_entity;
};

} // namespace RED_LILIUM_NAMESPACE
