#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <cstdint>
#include <memory>

#include <box2d/box2d.h>
#include <glm/glm.hpp>

namespace stardust
{
	namespace physics
	{
		class World
		{
		private:
			inline static std::uint32_t s_velocityIterations = 8u;
			inline static std::uint32_t s_positionIterations = 3u;

			std::unique_ptr<b2World> m_handle = nullptr;

		public:
			inline static std::uint32_t GetVelocityIterations() noexcept { return s_velocityIterations; }
			inline static void SetVelocityIterations(const std::uint32_t iterations) noexcept { s_velocityIterations = iterations; }
			inline static std::uint32_t GetPositionIterations() noexcept { return s_positionIterations; }
			inline static void SetPositionIterations(const std::uint32_t iterations) noexcept { s_positionIterations = iterations; }

			World() = default;
			World(const glm::vec2& gravity);
			~World() noexcept = default;

			void Initialise(const glm::vec2& gravity);

			inline bool IsValid() const noexcept { return m_handle != nullptr; }

			void Step(const float timestep) const;

			[[nodiscard]] inline b2World* const GetRawHandle() const noexcept { return m_handle.get(); }
		};
	}
}

#endif