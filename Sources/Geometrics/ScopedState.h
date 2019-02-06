#pragma once

#include <Render/RenderCommon.h>

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Context;

struct ScopedState
{
	ScopedState(ptr<Context> context);
	~ScopedState();

private:
	ptr<Context> m_context;
	std::vector<std::function<void()>> m_revert;
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
