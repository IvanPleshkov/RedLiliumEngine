#pragma once

#include <Core/Common.h>
#include <Core/ApplicationSettings.h>

namespace RED_LILIUM_NAMESPACE
{

class ImguiApplication : public RedLiliumObject
{
public:
	~ImguiApplication() override = default;

	i32 Start(ptr<ApplicationSettings> applicationSettings);

	virtual void Init() = 0;
	virtual void Tick(f32 dTime) = 0;
};

class ImguiDemoApplication : public ImguiApplication
{
public:
	void Init() override;
	void Tick(f32 dTime) override;
};

}
