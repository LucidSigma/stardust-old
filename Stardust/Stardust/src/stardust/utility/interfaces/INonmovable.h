#pragma once
#ifndef I_NONMOVABLE
#define I_NONMOVABLE

namespace stardust
{
	class INonmovable
	{
	protected:
		INonmovable() = default;

		INonmovable(INonmovable&&) noexcept = delete;
		INonmovable& operator =(INonmovable&&) noexcept = delete;

		~INonmovable() noexcept = default;
	};
}

#endif