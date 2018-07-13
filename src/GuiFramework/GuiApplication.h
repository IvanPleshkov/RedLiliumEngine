#pragma once

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "nanovg/nanovg.h"

#include "core/common.h"
#include "Widget.h"
#include "Panel.h"
#include "GuiManager.h"

#include <fstream>

namespace RED_LILIUM_NAMESPACE
{

class GuiApplication : public entry::AppI
{
public:
	GuiApplication(const char* _name, const char* _description)
		: entry::AppI(_name, _description)
	{
		LoadDataPath();
	}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		Args args(_argc, _argv);

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_TEXT;
		m_reset  = BGFX_RESET_VSYNC;

		bgfx::Init init;
		init.type     = args.m_type;
		init.vendorId = args.m_pciId;
		init.resolution.width  = m_width;
		init.resolution.height = m_height;
		init.resolution.reset  = m_reset;
		bgfx::init(init);

		// Enable debug text.
		bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);

		imguiCreate();

		m_nvg = nvgCreate(1, 0);
		bgfx::setViewMode(0, bgfx::ViewMode::Sequential);
		
		m_timeOffset = bx::getHPCounter();
	}

	int shutdown() override
	{
		nvgDelete(m_nvg);

		imguiDestroy();

		// Shutdown bgfx.
		bgfx::shutdown();

		return 0;
	}

	bool update() override
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
		{
			int64_t now = bx::getHPCounter();
			const double freq = double(bx::getHPFrequency());
			float time = (float)((now - m_timeOffset) / freq);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			nvgBeginFrame(m_nvg, m_width, m_height, 1.0f);

			// renderDemo(m_nvg, float(m_mouseState.m_mx), float(m_mouseState.m_my), float(m_width), float(m_height), time, 0, &m_data);
			draw();

			nvgEndFrame(m_nvg);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			return true;
		}

		return false;
	}


	virtual void draw()
	{
		nvgBeginPath(m_nvg);
		nvgRect(m_nvg, 100, 100, 120, 30);
		nvgFillColor(m_nvg, nvgRGBA(255, 192, 0, 255));
		nvgFill(m_nvg);
	}

private:
	void LoadDataPath()
	{
		std::string line;
		std::ifstream pathsFile("paths.json");
		if (pathsFile.is_open())
		{
			while (std::getline(pathsFile, line))
			{
				std::cout << line << '\n';
			}
			pathsFile.close();
		}
		m_dataPath = line;
	}

protected:
	NVGcontextPtr m_nvg;

	entry::MouseState m_mouseState;
	int64_t m_timeOffset;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	std::string m_dataPath;
};

}  // namespace RED_LILIUM_NAMESPACE
