#include "pch.h"
#include "Camera.h"

using namespace RED_LILIUM_NAMESPACE;

Camera::Camera()
	: m_view(1.0f)
	, m_proj(1.0f)
{}

const mat4& Camera::GetView() const
{
	return m_view;
}

void Camera::SetView(const mat4& view)
{
	m_view = view;
}

const mat4& Camera::GetProj() const
{
	return m_proj;
}

void Camera::SetProj(const mat4& proj)
{
	m_proj = proj;
}

void Camera::LookAt(const vec3& eye, const vec3& center, const vec3 up)
{
	m_view = glm::lookAt(eye, center, up);
}

void Camera::SetPerspective(float fov, float aspect, float znear, float zfar)
{
	m_proj = glm::perspective(fov, aspect, znear, zfar);
}

mat4 Camera::GetReversedZProj() const
{
	return dmat4();
}

mat4 Camera::MakeInfReversedZProjRH(f32 fovY_radians, f32 aspectWbyH, f32 zNear)
{
	f32 f = 1.0f / tan(fovY_radians / 2.0f);
	return dmat4(
		f / aspectWbyH, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, zNear, 0.0f);
}
