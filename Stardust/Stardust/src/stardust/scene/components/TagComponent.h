#pragma once
#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include <string>

namespace stardust
{
	namespace components
	{
		struct TagComponent
		{
			std::string name;

			TagComponent(const std::string& name = "")
				: name(name)
			{ }

			~TagComponent() noexcept = default;
		};
	}
}

#endif