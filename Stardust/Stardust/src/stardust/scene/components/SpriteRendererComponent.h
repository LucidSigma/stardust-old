#pragma once
#ifndef SPRITE_RENDERER_COMPONENT_H
#define SPRITE_RENDERER_COMPONENT_H

#include <optional>

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

			std::optional<stardust::rect::Rect> renderArea;

			SpriteRendererComponent()
				: texture(nullptr), z(0), renderArea(std::nullopt)
			{ }

			SpriteRendererComponent(const Texture& texture, const int z = 0, const decltype(renderArea)& renderArea = std::nullopt)
				: texture(&texture), z(z), renderArea(renderArea)
			{ }

			~SpriteRendererComponent() noexcept = default;
		};
	}
}

#endif#pragma once
