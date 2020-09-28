#pragma once
#ifndef SOUNDS_H
#define SOUNDS_H

#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#undef min
#undef max

#include "SoundBase.h"

namespace stardust
{
	using Sound = SoundBase<SoLoud::Wav>;
	using SoundStream = SoundBase<SoLoud::WavStream>;
}

#endif