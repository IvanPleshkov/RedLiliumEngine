#pragma once

#include <Render/RenderCommon.h>
#include "BuilderBase.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

template <class T>
class TransformBase : public BuilderBase<T>
{
public:
	TransformBase(Context& context)
		: BuilderBase(context)
	{}

	T& position(const vec3& position)
	{
		RED_LILIUM_NOT_IMPLEMENTED();
		return getThis();
	}

	T& scale(const vec3& scale)
	{
		RED_LILIUM_NOT_IMPLEMENTED();
		return getThis();
	}

	T& rotation(const quat& rotation)
	{
		RED_LILIUM_NOT_IMPLEMENTED();
		return getThis();
	}

	T& transform(const mat4& transform)
	{
		RED_LILIUM_NOT_IMPLEMENTED();
		return getThis();
	}

protected:
	vec3 m_position;
	vec3 m_scale;
	quat m_rotation;
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
