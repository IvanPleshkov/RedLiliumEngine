#pragma once

#include <Render/RenderCommon.h>
#include "../RenderState.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Context;

template <class T>
class BuilderBase : public NonCopyable
{
public:
	BuilderBase(Context& context)
		: m_renderState(context.GetRenderState())
	{}

protected:
	T& getThis()
	{
		return static_cast<T&>(*this);
	}

	RenderState m_renderState;
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
