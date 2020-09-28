#pragma once
#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <soloud/soloud.h>
#undef min
#undef max

namespace stardust
{
	class SoundSource
	{
	private:


	public:
		SoundSource(const SoLoud::handle handle);
		~SoundSource() noexcept;
	};
}

#endif