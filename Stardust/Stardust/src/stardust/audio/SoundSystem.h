#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "../utility/interfaces/INoncopyable.h"

#include <memory>

#include <soloud/soloud.h>
#undef min
#undef max

namespace stardust
{
	class SoundSystem
		: private INoncopyable
	{
	private:
		std::unique_ptr<SoLoud::Soloud> m_soLoudHandle;

	public:
		SoundSystem();
		SoundSystem(SoundSystem&& other) noexcept;
		SoundSystem& operator =(SoundSystem&& other) noexcept;
		~SoundSystem();

		void Update() const;

		void StopAllSounds() const;

		void FadeVolumeGlobal(const float volumeToFadeTo, const float seconds) const;

		void OscillateGlobalVolume(const float toVolume, const float fromVolume, const float frequency) const;
		void ResetGlobalOscillation() const;

		[[nodiscard]] unsigned int GetPlayingSoundCount() const;
		[[nodiscard]] bool IsSoundPlaying() const;

		[[nodiscard]] float GetGlobalVolume() const noexcept;
		void SetGlobalVolume(const float globalVolume) const noexcept;

		[[nodiscard]] float GetPostClipScaler() const noexcept;
		void SetPostClipScaler(const float postClipScaler) const noexcept;

		[[nodiscard]] float GetSpeedOfSound() const noexcept;
		void SetSpeedOfSound(const float speedOfSound) const noexcept;

		[[nodiscard]] SoLoud::Soloud& GetRawHandle() noexcept { return *m_soLoudHandle.get(); }

	private:
		void Initialise();
		void Destroy();
	};
}

#endif