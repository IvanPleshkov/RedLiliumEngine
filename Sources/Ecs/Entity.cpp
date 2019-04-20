#include "pch.h"
#include "Entity.h"
#include "Scene.h"

using namespace RED_LILIUM_NAMESPACE;

u64 Entity::Hash() const
{
	return *reinterpret_cast<ptr<const u64>>(this);
}

bool Entity::operator ==(const Entity& other) const
{
	return Hash() == other.Hash();
}

bool Entity::operator !=(const Entity& other) const
{
	return Hash() != other.Hash();
}

bool Entity::operator <(const Entity& other) const
{
	return Hash() < other.Hash();
}
