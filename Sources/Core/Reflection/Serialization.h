#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Serializator;

class Serializable : public RedLiliumObject
{
public:
	~Serializable() override = default;

	virtual void Serialize(ptr<Serializator> serializator) const = 0;
	virtual void Deserialize(ptr<const Serializator> serializator) = 0;

private:
};

class Serializator
{
public:
	class Environment;
	Serializator(ptr<Environment> environment, ptr<Serializator> parent);

private:
	ptr<Environment> m_environment;
};

json Serialize(ptr<const Serializable> object);
uptr<Serializable> Deserialize(const json& j);

#define RED_LILIUM_CLASS(...)

} // namespace RED_LILIUM_NAMESPACE
