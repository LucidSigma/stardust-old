#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../stardust/Stardust.h"

namespace sd = stardust;
namespace sd_comp = stardust::components;

struct KeyboardControlled
{
	float speed;

	KeyboardControlled(const float speed)
		: speed(speed)
	{ }
};

struct Velocity
{
	float x;
	float y;

	Velocity()
		: x(0.0f), y(0.0f)
	{ }

	Velocity(const float x, const float y)
		: x(x), y(y)
	{ }
};

struct Rotater
{
	float torque;

	Rotater()
		: torque(0.0f)
	{ }

	Rotater(const float torque)
		: torque(torque)
	{ }
};

class TestScene
	: public sd::Scene
{
private:
	sd::AssetManager<sd::Texture> m_textures;
	sd::AssetManager<sd::Sound> m_sounds;
	sd::AssetManager<sd::Font> m_fonts;

	sd::Entity m_drawable;
	sd::ParticleSystem m_particles;
	float m_particleCounter = 0.0f;

public:
	TestScene(sd::Application& application, const std::string& name)
		: sd::Scene(application, name)
	{ }

	virtual ~TestScene() noexcept override = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		const auto textures = sd::vfs::GetAllFilesInDirectory("assets/textures");

		for (const auto textureFile : textures)
		{
			const auto textureName = sd::vfs::GetFileStem(textureFile);
			m_textures.Add(textureName, m_application.GetRenderer(), textureFile);

			if (!m_textures[textureName].IsValid())
			{
				sd::Log::Error("Texture \"{}\" failed to load.", textureFile);

				return sd::Status::Fail;
			}

			m_textures[textureName].SetScaleMode(sd::Texture::ScaleMode::Nearest);
			sd::Log::Trace("Texture \"{}\" loaded successfully.", textureFile);
		}

		m_sounds.Add("test", "assets/sounds/test.wav");

		if (!m_sounds["test"].IsValid())
		{
			sd::Log::Error("Sound \"{}\" failed to load.", "assets/sounds/test.wav");

			return sd::Status::Fail;
		}

		sd::Log::Trace("Sound \"{}\" loaded successfully.", "assets/sounds/test.wav");

		m_fonts.Add("arial", "assets/fonts/arial.ttf", 64u);

		if (!m_fonts["arial"].IsValid())
		{
			sd::Log::Error("Font \"{}\" failed to load.", "assets/fonts/arial.ttf");

			return sd::Status::Fail;
		}

		m_textures.Add("text", sd::text::RenderTextWithOutline(m_application.GetRenderer(), m_fonts["arial"], "This is some text.", sd::colours::Pink, 4u, sd::colours::Purple));

		sd::Log::Trace("Font \"{}\" loaded successfully.", "assets/fonts/arial.ttf");

		m_drawable = CreateEntity();
		m_drawable.AddComponent<KeyboardControlled>(500.0f);
		m_drawable.AddComponent<sd_comp::TransformComponent>(
			glm::vec2{
				m_application.GetRenderer().GetLogicalSize().x / 2.0f - m_textures["gear"].GetSize().x / 2.0f,
				m_application.GetRenderer().GetLogicalSize().y / 2.0f - m_textures["gear"].GetSize().y / 2.0f,
			}
		);
		m_drawable.AddComponent<Velocity>(0.0f, 0.0f);
		m_drawable.AddComponent<Rotater>(250.0f);
		m_drawable.AddComponent<sd_comp::SpriteRendererComponent>(m_textures["gear"]);

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept override
	{
		m_textures.Clear();
		m_entityRegistry.clear();
	}

	virtual void FixedUpdate(const float fixedDeltaTime) override
	{
		const auto physicsEntities = m_entityRegistry.view<sd_comp::TransformComponent, Velocity, Rotater, sd_comp::SpriteRendererComponent>();

		physicsEntities.each([fixedDeltaTime, this](auto& transform, const auto& velocity, const auto& rotater, const auto& spriteRenderer)
		{
			transform.position.x += fixedDeltaTime * velocity.x;
			transform.position.y += fixedDeltaTime * velocity.y;

			transform.position.x = std::clamp(transform.position.x, 0.0f, static_cast<float>(m_application.GetRenderer().GetLogicalSize().x - spriteRenderer.texture->GetSize().x));
			transform.position.y = std::clamp(transform.position.y, 0.0f, static_cast<float>(m_application.GetRenderer().GetLogicalSize().y - spriteRenderer.texture->GetSize().y));

			if (velocity.x < 0.0f || velocity.y > 0.0f)
			{
				transform.rotation -= fixedDeltaTime * rotater.torque;
			}
			else if (velocity.x > 0.0f || velocity.y < 0.0f)
			{
				transform.rotation += fixedDeltaTime * rotater.torque;
			}
		});
	}

	virtual void ProcessInput() override
	{
		if (sd::Input::GetKeyboardState().IsKeyDown(sd::KeyCode::F11))
		{
			if (!m_application.GetWindow().IsFullscreen())
			{
				m_application.GetWindow().SetBorderless(true);
			}

			m_application.GetWindow().ToggleFullscreen();

			if (!m_application.GetWindow().IsFullscreen())
			{
				m_application.GetWindow().SetBorderless(false);
			}
		}

		sd::SoundSource soundSource;

		if (sd::Input::GetKeyboardState().IsKeyDown(sd::KeyCode::Space))
		{
			m_sounds["test"].SetVolume(m_application.GetVolumeManager()[sd::VolumeManager::GetMasterVolumeName()]);
			soundSource = m_application.GetSoundSystem().PlaySound(m_sounds["test"]);
		}
		else if (sd::Input::GetKeyboardState().IsKeyDown(sd::KeyCode::B)) 
		{
			m_sounds["test"].SetVolume(0.25f);
			soundSource = m_application.GetSoundSystem().PlaySound(m_sounds["test"]);
		}

		if (sd::Input::GetKeyboardState().IsKeyDown(sd::KeyCode::Enter))
		{
			m_particles.KillAllParticles();
		}

		if (soundSource.HasValidHandle())
		{
			soundSource.SetRelativePlaySpeed(0.4f);
			soundSource.FadePan(1.0f, 1.0f);
		}

		m_entityRegistry.view<KeyboardControlled, Velocity>().each([](const auto& keyboardController, auto& velocity)
		{
			velocity.x = 0.0f;
			velocity.y = 0.0f;

			const float multiplier = sd::Input::GetKeyboardState().IsKeyPressed(sd::KeyCode::LeftShift) ? 2.0f : 1.0f;

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::W, sd::KeyCode::Up }))
			{
				velocity.y -= keyboardController.speed * multiplier;
			}

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::S, sd::KeyCode::Down }))
			{
				velocity.y += keyboardController.speed * multiplier;
			}

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::A, sd::KeyCode::Left }))
			{
				velocity.x -= keyboardController.speed * multiplier;
			}

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::D, sd::KeyCode::Right }))
			{
				velocity.x += keyboardController.speed * multiplier;
			}
		});
	}

	virtual void Update(const float deltaTime) override
	{
		m_particleCounter += deltaTime;

		if (m_particleCounter >= 0.05f)
		{
			m_particleCounter = 0.0f;
			m_particles.Emit(sd::ParticleSystem::ParticleData{
				.initialPosition = m_application.GetRenderer().GetLogicalSize() / 2u,
				.initialRotation = 0.0f,
				.minVelocity = { -160.0f, -160.0f },
				.maxVelocity = { 160.0f, 160.0f },
				.velocityUpdateMultipler = 0.1f,
				.minAngularVelocity = 0.0f,
				.maxAngularVelocity = 180.0f,
				.angularVelocityUpdateMultipler = -0.05f,
				.minSize = { 10.0f, 10.0f },
				.maxSize = { 25.0f, 25.0f },
				.sizeUpdateMultipler = -0.25f,
				.keepAsSquare = true,
				.startColour = sd::colours::Maroon,
				.endColour = sd::colours::Orange,
				.texture = &m_textures["particle"],
				.minLifetime = 1.0f,
				.maxLifetime = 3.0f,
			});
		}

		m_particles.Update(deltaTime);
	}

	virtual void Render(const sd::Renderer& renderer) const override
	{
		m_entityRegistry.sort<sd_comp::SpriteRendererComponent>([](const auto& lhs, const auto& rhs)
		{
			return lhs.z < rhs.z;
		});

		const auto renderableEntities = m_entityRegistry.view<sd_comp::TransformComponent, sd_comp::SpriteRendererComponent>();

		renderableEntities.each([&renderer](auto& transform, auto& spriteRenderer)
		{
			renderer.DrawRotatedTexture(
				*spriteRenderer.texture,
				spriteRenderer.renderArea,
				transform.position,
				transform.scale,
				transform.rotation,
				std::nullopt
			);
		});

		renderer.DrawTexture(m_textures["text"], std::nullopt, glm::vec2{ 10.0f, 10.0f }, glm::vec2{ 1.0f, 1.0f });
		m_particles.Render(renderer);
	}
};

#endif