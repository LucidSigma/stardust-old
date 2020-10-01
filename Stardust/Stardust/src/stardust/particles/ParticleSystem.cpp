#include "ParticleSystem.h"

#include <glm/gtx/compatibility.hpp>

#include "../rect/Rect.h"
#include "../utility/random/Random.h"

namespace stardust
{
	ParticleSystem::ParticleSystem()
	{
		m_particlePool.resize(s_ParticleCount);
	}

	void ParticleSystem::Update(const float deltaTime)
	{
		for (auto& particle : m_particlePool)
		{
			if (!particle.isActive)
			{
				continue;
			}

			if (particle.lifetimeRemaining <= 0.0f)
			{
				particle.isActive = false;

				continue;
			}

			particle.lifetimeRemaining -= deltaTime;
			particle.velocity *= 1.0f + (particle.acceleration * deltaTime);
			particle.angularVelocity *= 1.0f + (particle.angularAcceleration * deltaTime);

			particle.position += particle.velocity * deltaTime;
			particle.rotation += particle.angularVelocity * deltaTime;
			particle.size *= 1.0f + (particle.sizeUpdateMultipler * deltaTime);

			particle.currentColour = Vec4ToColour(
				glm::lerp(
					ColourToVec4(particle.endColour),
					ColourToVec4(particle.startColour),
					particle.lifetimeRemaining / particle.lifetime
				)
			);
		}
	}

	void ParticleSystem::Render(const Renderer& renderer) const
	{
		for (const auto& particle : m_particlePool)
		{
			if (!particle.isActive)
			{
				continue;
			}

			const rect::Rect sourceRect = rect::Create(
				static_cast<int>(particle.position.x),
				static_cast<int>(particle.position.y),
				static_cast<unsigned int>(particle.size.x),
				static_cast<unsigned int>(particle.size.y)
			);

			if (particle.texture == nullptr)
			{
				renderer.DrawRect(sourceRect, particle.currentColour);
			}
			else
			{
				renderer.DrawRotatedTexture(*particle.texture, std::nullopt, particle.position, particle.size / static_cast<glm::vec2>(particle.texture->GetSize()), particle.rotation, std::nullopt);
			}
		}
	}

	void ParticleSystem::Emit(const ParticleData& particleData)
	{
		Particle& particle = m_particlePool[m_particlePoolIndex];

		particle.position = particleData.initialPosition;
		particle.rotation = particleData.initialRotation;

		particle.velocity.x = Random::GenerateFloat(particleData.minVelocity.x, particleData.maxVelocity.x);
		particle.velocity.y = Random::GenerateFloat(particleData.minVelocity.y, particleData.maxVelocity.y);
		particle.acceleration = particleData.acceleration;

		particle.angularVelocity = Random::GenerateFloat(particleData.minAngularVelocity, particleData.maxAngularVelocity);
		particle.angularAcceleration = particleData.angularAcceleration;

		particle.size.x = Random::GenerateFloat(particleData.minSize.x, particleData.maxSize.x);
		particle.size.y = particleData.keepAsSquare ? particle.size.x : Random::GenerateFloat(particleData.minSize.y, particleData.maxSize.y);
		particle.sizeUpdateMultipler = particleData.sizeUpdateMultipler;

		particle.currentColour = particleData.startColour;
		particle.startColour = particleData.startColour;
		particle.endColour = particleData.endColour;
		particle.texture = particleData.texture;

		particle.lifetime = Random::GenerateFloat(particleData.minLifetime, particleData.maxLifetime);
		particle.lifetimeRemaining = particle.lifetime;

		particle.isActive = true;

		--m_particlePoolIndex;
		m_particlePoolIndex %= s_ParticleCount;
	}
}