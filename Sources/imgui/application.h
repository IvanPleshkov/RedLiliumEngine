#pragma once

#include <Core/Common.h>
#include <Core/ApplicationSettings.h>
#include <SDL/SDL.h>

namespace RED_LILIUM_NAMESPACE
{

class ImguiApplication : public RedLiliumObject
{
public:
	~ImguiApplication() override = default;

	i32 Start(ptr<ApplicationSettings> applicationSettings);

	virtual void Init() = 0;
	virtual void Tick(f32 dTime) = 0;
	virtual void PollEvent(ptr<SDL_Event> event) = 0;

protected:
	void Stop();
	ptr<SDL_Window> GetMainWindow();

private:
	ptr<SDL_Window> m_mainWindow;
	bool m_isRunning = true;
};

class ImguiDemoApplication : public ImguiApplication
{
public:
	void Init() override;
	void Tick(f32 dTime) override;
	void PollEvent(ptr<SDL_Event> event) override;

private:
	bool m_showDemoWindow = true;
	bool m_showAnotherWindow = false;
};

}
