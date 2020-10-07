#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <cstddef>
#include <optional>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../graphics/Colour.h"
#include "../graphics/renderer/Renderer.h"
#include "../graphics/texture/Texture.h"
#include "../rect/Rect.h"

namespace stardust
{
	class ParticleSystem
	{
	public:
		struct ParticleData
		{
			glm::vec2 initialPosition;
			float initialRotation;
			
			glm::vec2 minVelocity;
			glm::vec2 maxVelocity;
			float velocityUpdateMultipler;

			float minAngularVelocity;
			float maxAngularVelocity;
			float angularVelocityUpdateMultipler;

			bool isAffectedByGravity;

			glm::vec2 minSize;
			glm::vec2 maxSize;
			float sizeUpdateMultipler;
			bool keepAsSquare;

			Colour startColour;
			Colour endColour;
			const Texture* texture;
			std::optional<rect::Rect> textureArea;

			float minLifetime;
			float maxLifetime;
		};

	private:
		struct Particle
		{
			glm::vec2 position{ 0.0f, 0.0f };
			float rotation = 0.0f;

			glm::vec2 velocity{ 0.0f, 0.0f };
			float velocityUpdateMultipler = 1.0f;

			float angularVelocity;
			float angularVelocityUpdateMultipler;

			bool isAffectedByGravity = false;

			glm::vec2 size{ 0.0f, 0.0f };
			float sizeUpdateMultipler = 1.0f;
			
			Colour currentColour = colours::White;
			Colour startColour = colours::White;
			Colour endColour = colours::White;
			const Texture* texture = nullptr;
			std::optional<rect::Rect> textureArea = std::nullopt;

			float lifetime = 0.0f;
			float lifetimeRemaining = 0.0f;

			bool isActive = false;
		};

		static constexpr std::size_t s_ParticleCount = 1'000u;

		std::vector<Particle> m_particlePool;
		std::size_t m_particlePoolIndex = s_ParticleCount - 1u;

		std::unordered_map<std::size_t, Particle*> m_activeParticles;

		glm::vec2 m_gravity = glm::vec2{ 0.0f, 0.0f };

	public:
		static constexpr std::size_t GetMaxParticleCount() noexcept { return s_ParticleCount; }

		ParticleSystem();
		~ParticleSystem() noexcept = default;

		void Update(const float deltaTime);
		void Render(const Renderer& renderer) const;

		void Emit(const ParticleData& particleData);
		void KillAllParticles();

		void RepositionAllActiveParticles(const glm::vec2& relativePosition);
		void ResizeAllActiveParticles(const float relativeScale);

		inline std::size_t GetActiveParticleCount() const noexcept { return m_activeParticles.size(); }

		inline const glm::vec2& GetGravity() const noexcept { return m_gravity; }
		inline void SetGravity(const glm::vec2& gravity) noexcept { m_gravity = gravity; }
	};
}

#endif