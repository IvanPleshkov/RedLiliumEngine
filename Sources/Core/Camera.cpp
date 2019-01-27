#include "pch.h"
#include "Camera.h"

using namespace RED_LILIUM_NAMESPACE;

const dmat4& Camera::GetView() const
{
	return m_view;
}

void Camera::SetView(const dmat4& view)
{
	m_view = view;
}

const dmat4& Camera::GetProj() const
{
	return m_proj;
}

void Camera::SetProj(const dmat4& proj)
{
	m_proj = proj;
}

dmat4 Camera::GetReversedZProj() const
{
	return dmat4();
}

dmat4 Camera::MakeInfReversedZProjRH(double fovY_radians, double aspectWbyH, double zNear)
{
	double f = 1.0f / tan(fovY_radians / 2.0f);
	return dmat4(
		f / aspectWbyH, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, zNear, 0.0f);
}
