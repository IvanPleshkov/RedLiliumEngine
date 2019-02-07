#include "pch.h"
#include "Context.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Geometrics;

Context::Context()
{}

Context::~Context()
{}

Line& Context::AddLine()
{
	m_builders.push_back(Line(*this));
	return std::get<Line>(m_builders.back());
}

Bullet& Context::AddBullet()
{
	m_builders.push_back(Bullet(*this));
	return std::get<Bullet>(m_builders.back());
}

Cube& Context::AddCube()
{
	m_builders.push_back(Cube(*this));
	return std::get<Cube>(m_builders.back());
}

Sphere& Context::AddSphere()
{
	m_builders.push_back(Sphere(*this));
	return std::get<Sphere>(m_builders.back());
}

const RenderState& Context::GetRenderState() const
{
	return m_renderState;
}

void Context::Prepare()
{
}
