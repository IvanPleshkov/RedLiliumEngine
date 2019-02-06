#include "pch.h"
#include "ScopedState.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Geometrics;

ScopedState::ScopedState(ptr<Context> context)
	: m_context(context)
{
}

ScopedState::~ScopedState()
{
	for (auto& revert : m_revert)
	{
		revert();
	}
	m_revert.clear();
}
