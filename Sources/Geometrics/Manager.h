#pragma once

#include <Render/RenderCommon.h>
#include <Core/Camera.h>

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Context;
class BatchedDrawCall;
class InstancedDrawCall;
class SimpleDrawCall;

class Manager : public RedLiliumObject
{
public:
	Manager();
	~Manager();

	uptr<Context> CreateContext();
	void SubmitContext(uptr<Context>&& context);

	void Tick();
	void Prepare(const Camera& camera);
	void Draw(ptr<RenderContext> renderContext);

	bool Enable();
	void Enable(bool value);

private:
	std::mutex m_createContextMutex;
	std::mutex m_submitContextMutex;
	std::mutex m_internalMutex;

	bool m_enable;
	std::vector<uptr<Context>> m_currentContexts;
	std::vector<uptr<Context>> m_collectedContexts;
	std::stack<uptr<Context>> m_contextPool;

	uptr<BatchedDrawCall> m_batchedDrawCalls;
	uptr<InstancedDrawCall> m_instancedDrawCalls;
	uptr<SimpleDrawCall> m_simpleDrawCalls;
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
