#pragma once
#ifndef SPRITE_RENDERER_COMPONENT_H
#define SPRITE_RENDERER_COMPONENT_H

#include <optional>

#include <glm/glm.hpp>

#include "../../graphics/texture/Texture.h"
#include "../../rect/Rect.h"

namespace stardust
{
	namespace components
	{
		struct SpriteRendererComponent
		{
			const Texture* texture;
			int z;

			glm::vec2 scale;

			std::optional<stardust::rect::Rect> renderArea;

			SpriteRendererComponent()
				: texture(nullptr), scale(glm::vec2{ 0.0f, 0.0f }), z(0), renderArea(std::nullopt)
			{ }

			SpriteRendererComponent(const Texture& texture, const glm::vec2& scale, const int z = 0, const decltype(renderArea)& renderArea = std::nullopt)
				: texture(&texture), scale(scale), z(z), renderArea(renderArea)
			{ }

			~SpriteRendererComponent() noexcept = default;
		};
	}
}

#endif#pragma once
