#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Camera : public RedLiliumObject
{
public:
	~Camera() override = default;

	const dmat4& GetView() const;
	void SetView(const dmat4& view);
	const dmat4& GetProj() const;
	void SetProj(const dmat4& proj);

	dmat4 GetReversedZProj() const;

	static dmat4 MakeInfReversedZProjRH(double fovY_radians, double aspectWbyH, double zNear);

private:
	dmat4 m_view;
	dmat4 m_proj;
};

} // namespace RED_LILIUM_NAMESPACE
