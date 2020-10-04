#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "../graphics/renderer/Renderer.h"
#include "../graphics/window/Window.h"

namespace stardust
{
	class Camera
	{
	private:
		glm::vec2 m_position{ 0.0f, 0.0f };
		float m_z = -10.0f;

		float m_rotation = 0.0f;
		float m_zoom = 1.0f;

		float m_halfSize = 0.0f;
		float m_aspectRatio = 0.0f;

	public:
		Camera() = default;
		Camera(const Renderer& renderer, const float halfSize);
		~Camera() noexcept = default;

		void Initialise(const Renderer& renderer, const float halfSize);

		inline glm::vec2& GetPosition() noexcept { return m_position; }
		inline const glm::vec2& GetPosition() const noexcept { return m_position; }
		inline void SetPosition(const glm::vec2& position) noexcept { m_position = position; }

		inline float GetZ() const noexcept { return m_z; }
		inline void SetZ(const float z) noexcept { m_z = z; }

		inline float GetRotation() const noexcept { return m_rotation; }
		inline void SetRotation(const float rotation) noexcept { m_rotation = rotation; }

		inline float GetZoom() const noexcept { return m_zoom; }
		inline void SetZoom(const float zoom) noexcept { m_zoom = zoom; }

		inline float GetHalfSize() const noexcept { return m_halfSize; }
		inline void SetHalfSize(const float halfSize) noexcept { m_halfSize = halfSize; }

		inline float GetAspectRatio() const noexcept { return m_aspectRatio; }

		[[nodiscard]] float GetPixelsPerUnit(const Window& window) const noexcept;
	};
}

#endif