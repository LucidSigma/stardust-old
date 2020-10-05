#include "ParticleSystem.h"

#include <cstdint>

#include <glm/gtx/compatibility.hpp>

#include "../utility/random/Random.h"

namespace stardust
{
	ParticleSystem::ParticleSystem()
	{
		m_particlePool.resize(s_ParticleCount);
	}

	void ParticleSystem::Update(const float deltaTime)
	{
		std::vector<std::size_t> particlesToRemove{ };

		for (auto& [particleID, particle] : m_activeParticles)
		{
			if (particle->lifetimeRemaining <= 0.0f)
			{
				particle->isActive = false;
			}

			if (!particle->isActive)
			{
				particlesToRemove.push_back(particleID);

				continue;
			}			

			particle->lifetimeRemaining -= deltaTime;
			particle->velocity *= 1.0f + (particle->velocityUpdateMultipler * deltaTime);
			particle->angularVelocity *= 1.0f + (particle->angularVelocityUpdateMultipler * deltaTime);

			if (particle->isAffectedByGravity)
			{
				particle->velocity += m_gravity * deltaTime;
			}

			particle->position += particle->velocity * deltaTime;
			particle->rotation += particle->angularVelocity * deltaTime;
			particle->size *= 1.0f + (particle->sizeUpdateMultipler * deltaTime);

			particle->currentColour = Vec4ToColour(
				glm::lerp(
					ColourToVec4(particle->endColour),
					ColourToVec4(particle->startColour),
					particle->lifetimeRemaining / particle->lifetime
				)
			);
		}

		for (const std::size_t particleID : particlesToRemove)
		{
			m_activeParticles.erase(particleID);
		}
	}

	void ParticleSystem::Render(const Renderer& renderer) const
	{
		for (const auto& [particleID, particle] : m_activeParticles)
		{
			if (!particle->isActive)
			{
				continue;
			}

			const rect::Rect sourceRect = rect::Create(
				static_cast<int>(particle->position.x - particle->size.x / 2.0f),
				static_cast<int>(particle->position.y - particle->size.y / 2.0f),
				static_cast<unsigned int>(particle->size.x),
				static_cast<unsigned int>(particle->size.y)
			);

			if (particle->texture == nullptr)
			{
				renderer.DrawRect(sourceRect, particle->currentColour);
			}
			else
			{
				const auto [originalRed, originalGreen, originalBlue] = particle->texture->GetColourMod();
				const std::uint8_t originalAlpha = particle->texture->GetAlphaMod();

				particle->texture->SetColourMod(particle->currentColour.r, particle->currentColour.g, particle->currentColour.b);
				particle->texture->SetAlphaMod(particle->currentColour.a);

				glm::vec2 particleScale = particle->size / static_cast<glm::vec2>(particle->texture->GetSize());

				if (particle->textureArea.has_value())
				{
					particleScale.x = particle->size.x / static_cast<float>(particle->textureArea->w);
					particleScale.y = particle->size.y / static_cast<float>(particle->textureArea->h);
				}

				renderer.DrawRotatedTexture(*particle->texture, particle->textureArea, particle->position, particleScale, particle->rotation);

				particle->texture->SetColourMod(originalRed, originalGreen, originalBlue);
				particle->texture->SetAlphaMod(originalAlpha);
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
		particle.velocityUpdateMultipler = particleData.velocityUpdateMultipler;

		particle.angularVelocity = Random::GenerateFloat(particleData.minAngularVelocity, particleData.maxAngularVelocity);
		particle.angularVelocityUpdateMultipler = particleData.angularVelocityUpdateMultipler;

		particle.isAffectedByGravity = particleData.isAffectedByGravity;

		particle.size.x = Random::GenerateFloat(particleData.minSize.x, particleData.maxSize.x);
		particle.size.y = particleData.keepAsSquare ? particle.size.x : Random::GenerateFloat(particleData.minSize.y, particleData.maxSize.y);
		particle.sizeUpdateMultipler = particleData.sizeUpdateMultipler;

		particle.currentColour = particleData.startColour;
		particle.startColour = particleData.startColour;
		particle.endColour = particleData.endColour;
		particle.texture = particleData.texture;
		particle.textureArea = particleData.textureArea;

		particle.lifetime = Random::GenerateFloat(particleData.minLifetime, particleData.maxLifetime);
		particle.lifetimeRemaining = particle.lifetime;

		particle.isActive = true;

		m_activeParticles.insert({ m_particlePoolIndex, &m_particlePool[m_particlePoolIndex] });

		--m_particlePoolIndex;
		m_particlePoolIndex %= s_ParticleCount;
	}

	void ParticleSystem::KillAllParticles()
	{
		m_activeParticles.clear();
	}
}