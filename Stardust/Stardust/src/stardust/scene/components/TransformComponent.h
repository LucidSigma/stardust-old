#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

namespace stardust
{
	namespace components
	{
		struct TransformComponent
		{
			glm::vec2 position;
			float rotation;
			glm::vec2 scale;

			TransformComponent()
				: position(glm::vec2{ 0.0f, 0.0f }), rotation(0.0f), scale(glm::vec2{ 1.0f, 1.0f })
			{ }

			TransformComponent(const glm::vec2& position, const float rotation = 0.0f, const glm::vec2& scale = glm::vec2{ 1.0f, 1.0f })
				: position(position), rotation(rotation), scale(scale)
			{ }

			~TransformComponent() noexcept = default;
		};
	}
}

#endif#pragma once
