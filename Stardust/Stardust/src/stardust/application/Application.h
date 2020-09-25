#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <atomic>
#include <cstdint>
#include <string>
#include <string_view>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../graphics/renderer/Renderer.h"
#include "../locale/Locale.h"
#include "../scene/SceneManager.h"
#include "../config/Config.h"
#include "../graphics/window/Window.h"

namespace stardust
{
	class Application final
		: private INoncopyable, private INonmovable
	{
	public:
		struct FilepathsInfo
		{
			std::string_view windowIcon;
			std::string_view config;
			std::string_view logFile;
			std::string_view screenshots;
		};

		struct FilesystemCreateInfo
		{
			const char* argv0;
			std::string_view assetsArchive;
			std::string_view localesArchive;
		};

		struct CreateInfo
		{
			std::string_view title;
			FilepathsInfo filepaths;
			FilesystemCreateInfo filesystem;

			float fixedTimestep;

			bool requireRenderToTexture;
			glm::uvec2 rendererLogicalSize;
		};

	private:
		bool m_didInitialiseSuccessfully = false;

		Config m_config;
		Locale m_locale;

		Window m_window;
		Renderer m_renderer;
		bool m_hasWindowFocus = true;

		std::atomic<bool> m_isRunning = true;

		float m_fixedTimestep = 0.0f;
		float m_deltaTime = 0.0f;
		std::uint64_t m_ticksCount = 0u;
		float m_elapsedTime = 0.0f;

		SceneManager m_sceneManager;
		bool m_isCurrentSceneFinished = false;

		entt::registry m_entityRegistry{ };

		std::string m_screenshotDirectory;

	public:
		Application(const CreateInfo& createInfo);
		~Application() noexcept;

		void Run();

		void FinishCurrentScene() noexcept;
		void ForceQuit() noexcept;

		void TakeScreenshot();

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		inline Config& GetConfig() noexcept { return m_config; }
		inline Locale& GetLocale() noexcept { return m_locale; }
		inline Window& GetWindow() noexcept { return m_window; }
		inline const Renderer& GetRenderer() const noexcept { return m_renderer; }
		inline SceneManager& GetSceneManager() noexcept { return m_sceneManager; }
		inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }

		[[nodiscard]] std::string GetPlatformName() const;

		inline bool IsRunning() const noexcept { return m_isRunning; }
		inline float GetElapsedTime() const noexcept { return m_elapsedTime; }
		inline bool HasWindowFocus() const noexcept { return m_hasWindowFocus; }

	private:
		void Initialise(const CreateInfo& createInfo);
		void InitialiseWindow(const CreateInfo& createInfo);
		void InitialiseScenes();

		void PollEvents(SDL_Event& event);
		void ProcessWindowEvents(const SDL_WindowEvent& windowEvent);

		void FixedUpdate();
		void ProcessInput();
		void Update();
		void LateUpdate();
		void Render() const;

		void CalculateDeltaTime();
		void UpdateSceneQueue();
	};
}
#endif