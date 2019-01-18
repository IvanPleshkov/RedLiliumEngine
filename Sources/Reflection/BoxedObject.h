#pragma once

#include <Core/Common.h>
#include "Serialization.h"

namespace RED_LILIUM_NAMESPACE
{

class BoxedObjectBase : public Serializable
{
public:
	RED_LILIUM_CLASS(BoxedObjectBase, Serializable);

	~BoxedObjectBase() override = default;
};

template<class T>
class BoxedObject : public BoxedObjectBase
{
public:
	RED_LILIUM_CLASS(BoxedObject<T>, BoxedObjectBase);

	BoxedObject(const T& value);
	BoxedObject(T&& value);
	~BoxedObject() override = default;

private:
	T m_value;
};

template<class T>
uptr<Serializable> Box(const T& value)
{
	return umake<BoxedObject>(value);
}

template<class T>
uptr<Serializable> Box(T&& value)
{
	return umake<BoxedObject>(std::move(value));
}

} // namespace RED_LILIUM_NAMESPACE
