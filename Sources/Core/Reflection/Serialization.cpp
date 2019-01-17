#include "pch.h"
#include "Serialization.h"

using namespace RED_LILIUM_NAMESPACE;

class Serializator::Environment : public RedLiliumObject
{
public:
	~Environment() override = default;
	json GetSerializedJson()
	{
		return {};
	}


};


json RED_LILIUM_NAMESPACE::Serialize(ptr<const Serializable> object)
{
	return {};
	// uptr<Serializator::Environment> environment = umake<Serializator::Environment>();
	// object->Serialize(rootSerializator);
	// return std::move(rootSerializator.GetSerializedJson());
}

uptr<Serializable> RED_LILIUM_NAMESPACE::Deserialize(const json& j)
{
	return nullptr;
}
