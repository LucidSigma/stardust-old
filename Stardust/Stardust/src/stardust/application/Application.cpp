#include "Application.h"

#include <chrono>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <magic_enum/magic_enum.hpp>
#include <SDL2/SDL_ttf.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "../debug/logging/Log.h"
#include "../debug/message_box/MessageBox.h"
#include "../graphics/surface/PixelSurface.h"
#include "../input/Input.h"
#include "../input/KeyCode.h"
#include "../scene/Scene.h"
#include "../text/text_input/TextInput.h"
#include "../utility/enums/Status.h"
#include "../vfs/VFS.h"

namespace stardust
{
	Application::Application(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Application::~Application() noexcept
	{
		m_entityRegistry.clear();

		Input::RemoveAllGameControllers();

		m_renderer.Destroy();
		m_window.Destroy();

		vfs::Quit();

		TTF_Quit();
		SDL_Quit();
	}

	void Application::Run()
	{
		InitialiseScenes();

		SDL_Event event{ };
		float timeAccumulator = 0.0f;
		
		while (m_isRunning)
		{
			CalculateDeltaTime();
			m_elapsedTime += m_deltaTime;
			timeAccumulator += m_deltaTime;

			m_soundSystem.Update();
			PollEvents(event);

			while (timeAccumulator >= m_fixedTimestep)
			{
				FixedUpdate();
				timeAccumulator -= m_fixedTimestep;
			}

			ProcessInput();
			Update();
			LateUpdate();

			Render();

			UpdateSceneQueue();
		}
	}

	void Application::FinishCurrentScene() noexcept
	{
		m_isCurrentSceneFinished = true;
	}

	void Application::ForceQuit() noexcept
	{
		m_isRunning = false;
	}

	void Application::TakeScreenshot()
	{
		const PixelSurface pixelData = m_renderer.ReadPixels();

		if (!pixelData.IsValid())
		{
			message_box::Show(m_locale["warnings"]["titles"]["screenshot"], m_locale["warnings"]["bodies"]["screenshot"], message_box::Type::Warning);
			Log::EngineWarn("Failed to take screenshot.");

			return;
		}
		
		const unsigned long long currentTime = static_cast<unsigned long long>(std::chrono::system_clock::now().time_since_epoch().count());
		const std::string screenshotFilename = m_screenshotDirectory + "/screenshot_" + std::to_string(currentTime) + ".png";

		if (stbi_write_png(
			screenshotFilename.c_str(),
			static_cast<int>(pixelData.GetSize().x),
			static_cast<int>(pixelData.GetSize().y),
			static_cast<int>(pixelData.GetPitch() / pixelData.GetSize().x),
			pixelData.GetPixels(),
			static_cast<int>(pixelData.GetPitch())
		) == 0)
		{
			message_box::Show(m_locale["warnings"]["titles"]["screenshot"], m_locale["warnings"]["bodies"]["screenshot"], message_box::Type::Warning);
			Log::EngineWarn("Failed to take screenshot.");
		}
		else
		{
			Log::EngineTrace("Screenshot captured at {}.", screenshotFilename);
		}
	}

	[[nodiscard]] std::string Application::GetPlatformName() const
	{
		return SDL_GetPlatform();
	}

	std::any& Application::GetFromGlobalSceneData(const std::string& dataName)
	{
		return m_globalSceneData[dataName];
	}

	void Application::RemoveFromGlobalSceneData(const std::string& dataName)
	{
		m_globalSceneData.erase(dataName);
	}

	void Application::Initialise(const CreateInfo& createInfo)
	{
	#ifndef NDEBUG
		Log::Initialise(createInfo.filepaths.logFile);
	#endif

		Log::EngineInfo("Logger initialised.");
		Log::EngineDebug("Platform detected: \"{}\".", GetPlatformName());
		Log::EngineInfo("ECS initialised.");

		static const std::vector<std::function<bool(Application* const, const CreateInfo&)>> initialisationFunctions{
			&Application::InitialiseVFS,
			&Application::InitialiseConfig,
			&Application::InitialiseLocale,
			&Application::InitialiseSoundSystem,
			&Application::InitialiseSDL,
			&Application::InitialiseWindow,
			&Application::InitialiseRenderer,
			&Application::InitialiseTextSystem,
		};

		for (const auto& initialisationFunction : initialisationFunctions)
		{
			if (!initialisationFunction(this, createInfo))
			{
				return;
			}
		}

		Input::SetGameControllerDeadzone(m_config["controls"]["controller-deadzone"]);

		m_screenshotDirectory = createInfo.filepaths.screenshots;
		m_fixedTimestep = createInfo.fixedTimestep;
		m_ticksCount = SDL_GetPerformanceCounter();

		m_didInitialiseSuccessfully = true;
	}

	bool Application::InitialiseVFS(const CreateInfo& createInfo)
	{
		if (!vfs::Initialise(createInfo.filesystem.argv0))
		{
			message_box::Show("Filesystem Error", "Virtual filesystem failed to initialise.", message_box::Type::Error);
			Log::EngineError("Failed to initialise virtual filesystem.");

			return false;
		}

		vfs::AddToSearchPath({ createInfo.filesystem.assetsArchive, createInfo.filesystem.localesArchive });
		Log::EngineInfo("Virtual filesystem initialised.");

		return true;
	}

	bool Application::InitialiseConfig(const CreateInfo& createInfo)
	{
		if (m_config.Initialise(createInfo.filepaths.config) == Status::Fail)
		{
			message_box::Show("Config Error", "Config file is invalid.", message_box::Type::Error);
			Log::EngineError("Failed to load config file at {}.", createInfo.filepaths.config);

			return false;
		}

		Log::EngineInfo("Config loaded.");

		return true;
	}

	bool Application::InitialiseLocale(const CreateInfo&)
	{
		m_locale.Initialise("locales/engine", "locales/client");

		if (m_locale.SetLocale(m_config["locale"]) == Status::Fail)
		{
			message_box::Show("Locale Error", "Failed to load initial locale files.", message_box::Type::Error);
			Log::EngineError("Failed to load locale files for initial locale {}.", m_config["locale"]);

			return false;
		}

		Log::EngineInfo("Locale \"{}\" loaded.", m_locale.GetCurrentLocaleName());

		return true;
	}

	bool Application::InitialiseSoundSystem(const CreateInfo&)
	{
		if (!m_soundSystem.DidInitialiseSuccessfully())
		{
			message_box::Show(m_locale["errors"]["titles"]["sound"], m_locale["errors"]["bodies"]["sound"], message_box::Type::Error);
			Log::EngineCritical("Failed to initialise sound system.");

			return false;
		}

		m_volumeManager.AddVolume(VolumeManager::GetMasterVolumeName(), m_config["audio"]["volumes"]["master"]);
		Log::EngineInfo("Sound system initialised.");

		return true;
	}

	bool Application::InitialiseSDL(const CreateInfo&)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			message_box::Show(m_locale["errors"]["titles"]["initialise"], m_locale["errors"]["bodies"]["initialise-sdl"], message_box::Type::Error);
			Log::EngineCritical("Failed to initialise SDL: {}.", SDL_GetError());

			return false;
		}

		Log::EngineInfo("SDL initialised.");

		return true;
	}

	bool Application::InitialiseWindow(const CreateInfo& createInfo)
	{
		Window::SetMinimiseOnFullscreenFocusLoss(m_config["graphics"]["enable-fullscreen-minimise"]);

		std::vector<Window::CreateFlag> windowCreateFlags{ Window::CreateFlag::AllowHighDPI, Window::CreateFlag::Shown };

		if (m_config["window"]["fullscreen"])
		{
			windowCreateFlags.push_back(Window::CreateFlag::HardFullscreen);

			if (m_config["window"]["borderless"])
			{
				windowCreateFlags.push_back(Window::CreateFlag::Borderless);
			}
		}

		m_window.Initialise(Window::CreateInfo{
			.title = createInfo.title.data(),
			.x = Window::Position::Centred,
			.y = Window::Position::Centred,
			.size = glm::uvec2{ m_config["window"]["size"]["width"], m_config["window"]["size"]["height"] },
			.flags = std::move(windowCreateFlags),
		});

		if (!m_window.IsValid())
		{
			message_box::Show(m_locale["errors"]["titles"]["window"], m_locale["errors"]["bodies"]["window"], message_box::Type::Error);
			Log::EngineCritical("Failed to create window: {}.", SDL_GetError());

			return false;
		}

		m_window.SetIcon(createInfo.filepaths.windowIcon, m_locale);
		Log::EngineInfo("Window created.");

		return true;
	}

	bool Application::InitialiseRenderer(const CreateInfo& createInfo)
	{
		m_renderer.Initialise(m_window, Renderer::CreateInfo{
			.presentVSync = m_config["frame-rate"]["enable-vsync"],
			.allowRenderToTexture = createInfo.requireRenderToTexture,
			.blendMode = Renderer::BlendMode::Alpha,
							  });

		if (!m_renderer.IsValid())
		{
			message_box::Show(m_locale["errors"]["titles"]["renderer"], m_locale["errors"]["bodies"]["renderer"], message_box::Type::Error);
			Log::EngineCritical("Failed to create renderer: {}.", SDL_GetError());

			return false;
		}

		m_renderer.SetLogicalSize(createInfo.rendererLogicalSize);
		Log::EngineInfo("Renderer created {} VSync.", m_config["frame-rate"]["enable-vsync"] ? "with" : "without");

		return true;
	}

	bool Application::InitialiseTextSystem(const CreateInfo& createInfo)
	{
		if (TTF_Init() != 0)
		{
			message_box::Show(m_locale["errors"]["titles"]["ttf"], m_locale["errors"]["bodies"]["ttf"], message_box::Type::Error);
			Log::EngineCritical("Failed to initialise SDL_TTF: {}.", TTF_GetError());

			return false;
		}

		Log::EngineInfo("Text subsystem initialised.");

		return true;
	}

	void Application::InitialiseScenes()
	{
		if (m_didInitialiseSuccessfully)
		{
			m_isRunning = !m_sceneManager.IsEmpty();

			if (m_isRunning)
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					message_box::Show(m_locale["errors"]["titles"]["scene"], m_locale["errors"]["bodies"]["initial-scene"], message_box::Type::Error);
					Log::EngineError("Failed to load initial scene.");
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Initial scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
				}
			}
			else [[unlikely]]
			{
				message_box::Show(m_locale["warnings"]["titles"]["scene"], m_locale["warnings"]["bodies"]["initial-scene"], message_box::Type::Warning);
				Log::EngineWarn("No initial scene loaded.");
			}
		}
		else
		{
			m_isRunning = false;
		}
	}

	void Application::PollEvents(SDL_Event& event)
	{
		Input::ResetScrollState();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_F2:
					TakeScreenshot();

					break;

				default:
					break;
				}

				if (text_input::IsActive() && magic_enum::enum_contains<KeyCode>(event.key.keysym.scancode))
				{
					m_sceneManager.CurrentScene()->OnKeyDown(static_cast<KeyCode>(event.key.keysym.scancode));
				}

				break;

			case SDL_KEYUP:
				if (text_input::IsActive() && magic_enum::enum_contains<KeyCode>(event.key.keysym.scancode))
				{
					m_sceneManager.CurrentScene()->OnKeyUp(static_cast<KeyCode>(event.key.keysym.scancode));
				}

				break;

			case SDL_TEXTINPUT:
				if (text_input::IsActive())
				{
					m_sceneManager.CurrentScene()->OnTextInput(std::string(event.text.text));
				}

				break;

			case SDL_MOUSEWHEEL:
				Input::UpdateScrollState(event.wheel.y);

				break;

			case SDL_WINDOWEVENT:
				ProcessWindowEvents(event.window);

				break;

			case SDL_CONTROLLERDEVICEADDED:
				if (Input::GameController* const gameController = Input::AddGameController(event.cdevice.which, m_locale); 
					gameController != nullptr)
				{
					m_sceneManager.CurrentScene()->OnGameControllerAdded(*gameController);
				}

				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				if (Input::GameController* const gameController = Input::GetGameController(event.cdevice.which); 
					gameController != nullptr)
				{
					m_sceneManager.CurrentScene()->OnGameControllerRemoved(*gameController);
				}

				Input::RemoveGameController(event.cdevice.which);

				break;

			case SDL_QUIT:
				m_isRunning = false;

				break;

			default:
				break;
			}
		}
	}

	void Application::ProcessWindowEvents(const SDL_WindowEvent& windowEvent)
	{
		switch (windowEvent.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_window.ProcessResize(glm::uvec2{ windowEvent.data1, windowEvent.data2 });
			m_sceneManager.CurrentScene()->OnWindowResized(glm::uvec2{ windowEvent.data1, windowEvent.data2 });

			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			m_sceneManager.CurrentScene()->OnWindowMaximised();

			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			m_sceneManager.CurrentScene()->OnWindowMinimised();

			[[fallthrough]];

		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_hasWindowFocus = false;

			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_hasWindowFocus = true;

			if (Input::IsMouseInRelativeMode())
			{
				Input::ClearRelativeMouseState();
			}

			break;

		default:
			break;
		}
	}

	void Application::FixedUpdate()
	{
		m_sceneManager.CurrentScene()->FixedUpdate(m_fixedTimestep);
	}

	void Application::ProcessInput()
	{
		if (m_hasWindowFocus)
		{
			Input::UpdateKeyboardState();
			Input::UpdateMouseState();
			Input::UpdateGameControllers();
		}

		if (Input::GetKeyboardState().IsKeyDown(KeyCode::Escape))
		{
			FinishCurrentScene();
		}
			
		m_sceneManager.CurrentScene()->ProcessInput();
	}

	void Application::Update()
	{
		m_sceneManager.CurrentScene()->Update(m_deltaTime);
	}

	void Application::LateUpdate()
	{
		m_sceneManager.CurrentScene()->LateUpdate(m_deltaTime);
	}

	void Application::Render() const
	{
		m_renderer.Clear(colours::Black);
		m_sceneManager.CurrentScene()->Render(m_renderer);
		m_renderer.Present();
	}

	void Application::CalculateDeltaTime()
	{
		static const bool capFramerate = m_config["frame-rate"]["cap-fps"];
		static const float fpsLimit = m_config["frame-rate"]["fps-limit"];

		const std::uint64_t newTicks = SDL_GetPerformanceCounter();
		const std::uint64_t frameTicks = newTicks - m_ticksCount;
		m_deltaTime = static_cast<float>(frameTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
		
		if (capFramerate)
		{
			static const float timeToWait = 1.0f / fpsLimit;

			if (m_deltaTime < timeToWait)
			{
				constexpr float MillisecondsPerSecond = 1'000.0f;

				SDL_Delay(static_cast<std::uint32_t>((timeToWait - m_deltaTime) * MillisecondsPerSecond));
			}

			m_deltaTime = timeToWait;
		}

		m_ticksCount = newTicks;
	}

	void Application::UpdateSceneQueue()
	{
		if (m_isCurrentSceneFinished)
		{
			Log::EngineTrace("Scene \"{}\" finished.", m_sceneManager.CurrentScene()->GetName());
			m_sceneManager.CurrentScene()->OnUnload();
			m_sceneManager.PopScene();
			m_entityRegistry.clear();
			m_soundSystem.GetListener().Reset();

			if (!m_sceneManager.IsEmpty())
			{
				if (m_sceneManager.CurrentScene()->OnLoad() == Status::Fail)
				{
					message_box::Show(m_locale["errors"]["titles"]["scene"], m_locale["errors"]["bodies"]["next-scene"], message_box::Type::Error);
					Log::EngineError("Failed to load scene.");
					m_isRunning = false;
				}
				else
				{
					Log::EngineTrace("Scene \"{}\" loaded.", m_sceneManager.CurrentScene()->GetName());
				}
			}

			m_isCurrentSceneFinished = false;
		}

		if (m_sceneManager.IsEmpty())
		{
			m_isRunning = false;
		}
	}
}