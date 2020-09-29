#pragma once
#ifndef LISTENER_H
#define LISTENER_H

#include "../utility/interfaces/INoncopyable.h"

#include <glm/glm.hpp>

namespace stardust
{
	class Listener
		: private INoncopyable
	{
	private:
		class SoundSystem* m_soundSystem = nullptr;

		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_lookAt{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_upVector{ 0.0f, 1.0f, 0.0f };

		glm::vec3 m_velocity{ 0.0f, 0.0f, 0.0f };

	public:
		Listener(SoundSystem& soundSystem);
		Listener(Listener&& other) noexcept;
		Listener& operator =(Listener&& other) noexcept;
		~Listener() noexcept = default;

		void Reset() noexcept;

		inline const glm::vec3& GetPosition() const noexcept { return m_position; }
		void SetPosition(const glm::vec3& position) noexcept;

		inline const glm::vec3& GetLookAt() const noexcept { return m_lookAt; }
		void SetLookAt(const glm::vec3& lookAt) noexcept;

		inline const glm::vec3& GeUpVector() const noexcept { return m_upVector; }
		void SetUpVector(const glm::vec3& upVector) noexcept;

		inline const glm::vec3& GetVelocity() const noexcept { return m_velocity; }
		void SetVelocity(const glm::vec3& velocity) noexcept;
	};
}

#endif