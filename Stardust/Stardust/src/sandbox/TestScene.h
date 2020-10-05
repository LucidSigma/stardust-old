#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../stardust/Stardust.h"

namespace sd = stardust;
namespace sd_comp = stardust::components;
namespace sd_phys = stardust::physics;

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
	float m_clickParticleDelay = 0.01f;

	sd::TextureAtlas m_textureAtlas;
	
	sd_phys::World m_physicsWorld;

	sd::Camera m_camera;

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
		m_drawable.AddComponent<KeyboardControlled>(6.0f);
		m_drawable.AddComponent<sd_comp::TransformComponent>(
			glm::vec2{
				0.0f,
				0.0f,
			}
		);
		m_drawable.AddComponent<Velocity>(0.0f, 0.0f);
		m_drawable.AddComponent<Rotater>(360.0f);
		m_drawable.AddComponent<sd_comp::SpriteRendererComponent>(m_textures["gear"], glm::vec2{ 1.0f, 1.0f });

		m_particles.SetGravity(glm::vec2{ 0.0f, 250.0f });

		m_textureAtlas.Initialise(m_application.GetRenderer(), "assets/textures/texture_atlases/conveyors.taj");
		m_physicsWorld.Initialise(glm::vec2{ 0.0f, -9.81f });

		m_camera.Initialise(m_application.GetRenderer(), 8.0f);

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

			transform.position.x = std::clamp(transform.position.x, -m_camera.GetHalfSize(), m_camera.GetHalfSize());
			transform.position.y = std::clamp(transform.position.y, -m_camera.GetHalfSize() / m_camera.GetAspectRatio(), m_camera.GetHalfSize() / m_camera.GetAspectRatio());

			if (velocity.x < 0.0f || velocity.y > 0.0f)
			{
				transform.rotation -= fixedDeltaTime * rotater.torque;
			}
			else if (velocity.x > 0.0f || velocity.y < 0.0f)
			{
				transform.rotation += fixedDeltaTime * rotater.torque;
			}
		});

		m_physicsWorld.Step(fixedDeltaTime);
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

		if (sd::Input::GetMouseState().IsButtonPressed(sd::Input::Mouse::Button::Left) && m_clickParticleDelay < 0.0f)
		{
			m_clickParticleDelay = 0.01f;

			auto finalColour = sd::colours::Yellow;
			finalColour.a = 0u;

			m_particles.Emit(sd::ParticleSystem::ParticleData{
				.initialPosition = sd::Input::GetMouseState().GetProportionalCoordinates(m_application.GetRenderer()),
				.initialRotation = 0.0f,
				.minVelocity = { -100.0f, -400.0f },
				.maxVelocity = { 100.0f, -10.0f },
				.velocityUpdateMultipler = 0.4f,
				.minAngularVelocity = 0.0f,
				.maxAngularVelocity = 180.0f,
				.angularVelocityUpdateMultipler = -0.1f,
				.isAffectedByGravity = false,
				.minSize = { 20.0f, 20.0f },
				.maxSize = { 40.0f, 40.0f },
				.sizeUpdateMultipler = -0.2f,
				.keepAsSquare = true,
				.startColour = sd::colours::Red,
				.endColour = finalColour,
				.texture = nullptr,
				.textureArea = std::nullopt,
				.minLifetime = 0.5f,
				.maxLifetime = 1.0f,
			});
		}

		m_entityRegistry.view<KeyboardControlled, Velocity>().each([](const auto& keyboardController, auto& velocity)
		{
			velocity.x = 0.0f;
			velocity.y = 0.0f;

			const float multiplier = sd::Input::GetKeyboardState().IsKeyPressed(sd::KeyCode::LeftShift) ? 2.0f : 1.0f;

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::W, sd::KeyCode::Up }))
			{
				velocity.y += keyboardController.speed * multiplier;
			}

			if (sd::Input::GetKeyboardState().IsAnyKeyPressed({ sd::KeyCode::S, sd::KeyCode::Down }))
			{
				velocity.y -= keyboardController.speed * multiplier;
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
		m_clickParticleDelay -= deltaTime;

		m_entityRegistry.view<sd_comp::TransformComponent, sd_comp::SpriteRendererComponent>().each([this, deltaTime](const auto& transform, const auto& spriteRenderer)
		{
			if (m_particleCounter >= 0.1f)
			{
				m_particleCounter = 0.0f;
				m_particles.Emit(sd::ParticleSystem::ParticleData{
					.initialPosition = m_camera.WorldSpaceToScreenSpace(transform.position),
					.initialRotation = 0.0f,
					.minVelocity = { -160.0f, -160.0f },
					.maxVelocity = { 160.0f, 160.0f },
					.velocityUpdateMultipler = 0.4f,
					.minAngularVelocity = 0.0f,
					.maxAngularVelocity = 180.0f,
					.angularVelocityUpdateMultipler = -0.05f,
					.isAffectedByGravity = true,
					.minSize = { 10.0f, 10.0f },
					.maxSize = { 25.0f, 25.0f },
					.sizeUpdateMultipler = -0.25f,
					.keepAsSquare = true,
					.startColour = sd::colours::Grey,
					.endColour = sd::colours::White,
					.texture = &m_textures["particle"],
					.textureArea = std::nullopt,
					.minLifetime = 0.5f,
					.maxLifetime = 3.25f,
				});
			}
		});
		
		m_particles.Update(deltaTime);
	}

	virtual void Render(const sd::Renderer& renderer) const override
	{
		m_particles.Render(renderer);

		m_entityRegistry.sort<sd_comp::SpriteRendererComponent>([](const auto& lhs, const auto& rhs)
		{
			return lhs.z < rhs.z;
		});

		const auto renderableEntities = m_entityRegistry.view<sd_comp::TransformComponent, sd_comp::SpriteRendererComponent>();

		renderableEntities.each([this, &renderer](auto& transform, auto& spriteRenderer)
		{
			renderer.DrawRotatedTexture(
				*spriteRenderer.texture,
				spriteRenderer.renderArea,
				m_camera.WorldSpaceToScreenSpace(transform.position),
				transform.scale,
				transform.rotation
			);
		});

		renderer.DrawTexture(m_textureAtlas.GetTexture(), m_textureAtlas["left"], m_camera.WorldSpaceToScreenSpace(glm::vec2{ -6.0f, 3.0f }), glm::vec2{ 4.0f, 4.0f });
		renderer.DrawTexture(m_textureAtlas.GetTexture(), m_textureAtlas["right"], m_camera.WorldSpaceToScreenSpace(glm::vec2{ -5.0f, 3.0f }), glm::vec2{ 4.0f, 4.0f });

		renderer.DrawTexture(m_textures["text"], std::nullopt, glm::vec2{ 10.0f, 10.0f }, glm::vec2{ 1.0f, 1.0f });
	}
};

#endif