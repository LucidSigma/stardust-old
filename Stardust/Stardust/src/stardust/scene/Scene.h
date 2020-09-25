#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <string>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../input/KeyCode.h"
#include "../input/Input.h"
#include "../graphics/renderer/Renderer.h"
#include "../utility/enums/Enums.h"

namespace stardust
{
	class Scene
		: private INoncopyable, private INonmovable
	{
	private:
		const std::string m_name;

	protected:
		class Application& m_application;
		entt::registry& m_entityRegistry;

	public:
		Scene(class Application& application, const std::string& name);
		virtual ~Scene() noexcept = default;

		[[nodiscard]] virtual Status OnLoad() = 0;
		virtual void OnUnload() noexcept = 0;

		inline virtual void OnWindowMinimised() { }
		inline virtual void OnWindowMaximised() { }
		inline virtual void OnWindowMoved(const glm::ivec2& newPosition) { }
		inline virtual void OnWindowResized(const glm::uvec2& newSize) { }

		inline virtual void OnKeyDown(const KeyCode key) { }
		inline virtual void OnKeyUp(const KeyCode key) { }
		inline virtual void OnTextInput(const std::string& text) { }

		inline virtual void OnGameControllerAdded(const Input::GameController& gameController) { }
		inline virtual void OnGameControllerRemoved(const Input::GameController& gameController) { }

		inline virtual void FixedUpdate(const float fixedDeltaTime) { };
		virtual void ProcessInput() = 0;
		virtual void Update(const float deltaTime) = 0;
		inline virtual void LateUpdate(const float deltaTime) { }

		virtual void Render(const Renderer& renderer) const = 0;

		inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }
		inline const std::string& GetName() const noexcept { return m_name; }

	protected:
		class Entity CreateEntity();
	};
}

#endif