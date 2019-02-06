#include "pch.h"
#include "Manager.h"
#include "Context.h"
#include "DrawCalls/BatchedDrawCall.h"
#include "DrawCalls/InstancedDrawCall.h"
#include "DrawCalls/SimpleDrawCall.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Geometrics;

Manager::Manager()
{}

Manager::~Manager()
{
}

uptr<Context> Manager::CreateContext()
{
	return nullptr;
}

void Manager::SubmitContext(uptr<Context>&& context)
{
}

void Manager::Tick()
{
}

void Manager::Prepare(const Camera& camera)
{
}

void Manager::Draw(ptr<RenderContext> renderContext)
{
}

bool Manager::Enable()
{
	return m_enable;
}

void Manager::Enable(bool value)
{
	std::scoped_lock lock(m_internalMutex);
	m_enable = value;
}
