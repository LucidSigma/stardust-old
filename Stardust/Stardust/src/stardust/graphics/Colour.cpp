#include "Colour.h"

#include <cstdint>

namespace stardust
{
	[[nodiscard]] glm::vec4 ColourToVec4(const Colour& colour)
	{
		return glm::vec4{
			colour.r,
			colour.g,
			colour.b,
			colour.a,
		};
	}

	[[nodiscard]] Colour Vec4ToColour(const glm::vec4& vector)
	{
		return Colour{
			static_cast<std::uint8_t>(vector.x),
			static_cast<std::uint8_t>(vector.y),
			static_cast<std::uint8_t>(vector.z),
			static_cast<std::uint8_t>(vector.w),
		};
	}
}