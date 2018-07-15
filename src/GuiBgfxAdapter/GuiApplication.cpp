#include "pch.h"
#include "GuiApplication.h"

// bgfx
#include <bx/uint32_t.h>
#include <common.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include <nanovg/nanovg.h>

#include "BgfxWindow.h"
#include "BgfxEnvironment.h"

using namespace RED_LILIUM_NAMESPACE;

class GuiApplication::Impl : public entry::AppI
{
public:
	GuiApplication::Impl(ptr<GuiApplication> application, const char* _name, const char* _description)
		: entry::AppI(_name, _description)
		, m_application(application)
	{ }

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;
	int shutdown() override;
	bool update() override;

	MouseState GetMouseState(entry::MouseState bgfxState);
	KeyState GetKeyState();

protected:
	ptr<GuiApplication> m_application;

	NVGcontextPtr m_nvg;
	int64_t m_timeOffset;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
};

//============================= GuiApplication

GuiApplication::GuiApplication(
	const std::string& name,
	const std::string& description,
	const std::vector<std::string>& args,
	vec2 windowSize)
	: m_args(args)
	, m_name(name)
	, m_description(description)
{
	InitDataPath();

	m_impl = std::make_unique<Impl>(this, m_name.c_str(), m_description.c_str());
	m_nativeWindow = std::make_unique<BgfxWindow>();
	m_nativeEnvironment = std::make_unique<BgfxEnvironment>();
	m_guiManager = std::make_unique<GuiManager>(m_nativeEnvironment.get());
}

void GuiApplication::Run()
{
	std::vector<const char*> argv;
	argv.reserve(m_args.size());
	for (auto& arg : m_args)
	{
		argv.push_back(arg.c_str());
	}
	entry::runApp(m_impl.get(), m_args.size(), &argv[0]);
}

const std::string& GuiApplication::GetDataPath() const
{
	return m_dataPath;
}

ptr<GuiManager> GuiApplication::GetGuiManager()
{
	return m_guiManager.get();
}

ptr<const GuiManager> GuiApplication::GetGuiManager() const
{
	return m_guiManager.get();
}

ptr<BgfxWindow> GuiApplication::GetNativeWindow()
{
	return m_nativeWindow.get();
}

ptr<const BgfxWindow> GuiApplication::GetNativeWindow() const
{
	return m_nativeWindow.get();
}

ptr<BgfxEnvironment> GuiApplication::GetNativeEnvironment()
{
	return m_nativeEnvironment.get();
}

ptr<const BgfxEnvironment> GuiApplication::GetNativeEnvironment() const
{
	return m_nativeEnvironment.get();
}

void GuiApplication::InitDataPath()
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

//============================= GuiApplication::Impl

void GuiApplication::Impl::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
{
	Args args(_argc, _argv);

	m_width = _width;
	m_height = _height;
	m_debug = BGFX_DEBUG_TEXT;
	m_reset = BGFX_RESET_VSYNC;

	bgfx::Init init;
	init.type = args.m_type;
	init.vendorId = args.m_pciId;
	init.resolution.width = m_width;
	init.resolution.height = m_height;
	init.resolution.reset = m_reset;
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

	entry::WindowState windowState;
	m_application->GetNativeWindow()->UpdateWindowState(windowState, m_nvg);

	m_application->OnBeginApplication();
}

int GuiApplication::Impl::shutdown()
{
	m_application->OnEndApplication();

	nvgDelete(m_nvg);

	imguiDestroy();

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool GuiApplication::Impl::update()
{
	entry::MouseState bgfxMouseState;
	if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &bgfxMouseState))
	{
		const int64_t now = bx::getHPCounter();
		const double freq = double(bx::getHPFrequency());
		double time = static_cast<double>(now - m_timeOffset) / freq;

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		nvgBeginFrame(m_nvg, m_width, m_height, 1.0f);

		m_application->m_mouseState = GetMouseState(bgfxMouseState);
		m_application->m_keyState = GetKeyState();
		m_application->m_guiManager->BeginFrame(time, m_application->m_mouseState, m_application->m_keyState);

		m_application->Update();

		m_application->m_guiManager->EndFrame();
		m_application->m_guiManager->Draw();

		nvgEndFrame(m_nvg);

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		return true;
	}

	return false;
}

MouseState GuiApplication::Impl::GetMouseState(entry::MouseState bgfxState)
{
	MouseState mouseState;
	mouseState.mousePosition = { bgfxState.m_mx, bgfxState.m_my };
	if (bgfxState.m_buttons[entry::MouseButton::Left])
	{
		mouseState.pressedKeys.Add(MouseKey::Left);
	}
	if (bgfxState.m_buttons[entry::MouseButton::Right])
	{
		mouseState.pressedKeys.Add(MouseKey::Right);
	}
	if (bgfxState.m_buttons[entry::MouseButton::Middle])
	{
		mouseState.pressedKeys.Add(MouseKey::Middle);
	}

	return mouseState;
}

KeyState GuiApplication::Impl::GetKeyState()
{
	return KeyState();
}
