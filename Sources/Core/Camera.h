#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

struct Camera
{
	Camera();

	const mat4& GetView() const;
	void SetView(const mat4& view);
	const mat4& GetProj() const;
	void SetProj(const mat4& proj);

	void LookAt(const vec3& eye, const vec3& center, const vec3 up);
	void SetPerspective(float fov, float aspect, float znear, float zfar);

	mat4 GetReversedZProj() const;

	static mat4 MakeInfReversedZProjRH(f32 fovY_radians, f32 aspectWbyH, f32 zNear);

private:
	mat4 m_view;
	mat4 m_proj;
};

} // namespace RED_LILIUM_NAMESPACE
