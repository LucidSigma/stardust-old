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
	: public stardust::Scene
{
private:
	stardust::AssetManager<stardust::Texture> m_textures;
	stardust::AssetManager<stardust::Sound> m_sounds;

	stardust::Entity m_drawable;

public:
	TestScene(stardust::Application& application, const std::string& name)
		: stardust::Scene(application, name)
	{ }

	virtual ~TestScene() noexcept override = default;

	[[nodiscard]] virtual stardust::Status OnLoad() override
	{
		const std::initializer_list<std::pair<std::string, std::string_view>> textureData{
			{ "gear", "assets/textures/gear.png" },
		};

		for (const auto& [textureName, texturePath] : textureData)
		{
			m_textures.Add(textureName, m_application.GetRenderer(), texturePath);

			if (!m_textures[textureName].IsValid())
			{
				stardust::Log::Error("Texture \"{}\" failed to load.", "assets/textures/gear.png");

				return stardust::Status::Fail;
			}
			
			m_textures[textureName].SetScaleMode(stardust::Texture::ScaleMode::Nearest);
			stardust::Log::Trace("Texture \"{}\" loaded successfully.", "assets/textures/gear.png");
		}

		m_sounds.Add("test", "assets/sounds/test.wav");

		if (!m_sounds["test"].IsValid())
		{
			return stardust::Status::Fail;
		}

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

		return stardust::Status::Success;
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
		if (stardust::Input::GetKeyboardState().IsKeyDown(stardust::KeyCode::F11))
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

		stardust::SoundSource soundSource;

		if (stardust::Input::GetKeyboardState().IsKeyDown(stardust::KeyCode::Space))
		{
			m_sounds["test"].SetVolume(1.0f);
			soundSource = m_application.GetSoundSystem().PlaySound(m_sounds["test"]);
		}
		else if (stardust::Input::GetKeyboardState().IsKeyDown(stardust::KeyCode::B))
		{
			m_sounds["test"].SetVolume(0.25f);
			soundSource = m_application.GetSoundSystem().PlaySound(m_sounds["test"]);
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

			const float multiplier = stardust::Input::GetKeyboardState().IsKeyPressed(stardust::KeyCode::LeftShift) ? 2.0f : 1.0f;

			if (stardust::Input::GetKeyboardState().IsAnyKeyPressed({ stardust::KeyCode::W, stardust::KeyCode::Up }))
			{
				velocity.y -= keyboardController.speed * multiplier;
			}

			if (stardust::Input::GetKeyboardState().IsAnyKeyPressed({ stardust::KeyCode::S, stardust::KeyCode::Down }))
			{
				velocity.y += keyboardController.speed * multiplier;
			}

			if (stardust::Input::GetKeyboardState().IsAnyKeyPressed({ stardust::KeyCode::A, stardust::KeyCode::Left }))
			{
				velocity.x -= keyboardController.speed * multiplier;
			}

			if (stardust::Input::GetKeyboardState().IsAnyKeyPressed({ stardust::KeyCode::D, stardust::KeyCode::Right }))
			{
				velocity.x += keyboardController.speed * multiplier;
			}
		});
	}

	virtual void Update(const float deltaTime) override
	{
		
	}

	virtual void Render(const stardust::Renderer& renderer) const override
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
	}
};

#endif