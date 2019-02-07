#pragma once

#include <Render/RenderCommon.h>
#include "RenderState.h"
#include "Builders/Line.h"
#include "Builders/Bullet.h"
#include "Builders/Cube.h"
#include "Builders/Sphere.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Context : public RedLiliumObject
{
public:
	Context();
	~Context() override;

	Line& AddLine();
	Bullet& AddBullet();
	Cube& AddCube();
	Sphere& AddSphere();

/*internal*/ public:
	const RenderState& GetRenderState() const;

	friend class ScopedState;
private:
	using BuilderVariant = std::variant<
		Line,
		Bullet,
		Cube,
		Sphere>;

	std::deque<BuilderVariant> m_builders;
	RenderState m_renderState;
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
