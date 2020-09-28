#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <memory>

#include <soloud/soloud.h>
#undef min
#undef max

#include "Listener.h"
#include "sounds/Sound.h"

namespace stardust
{
	class SoundSystem
		: private INoncopyable, private INonmovable
	{
	private:
		std::unique_ptr<SoLoud::Soloud> m_soLoudHandle;
		Listener m_listener;

	public:
		SoundSystem();
		~SoundSystem();

		void Update() const;

		// Play sound and return source when playing.
		/*AudioSource*/ void PlaySound(Sound& sound, const bool startPaused = false) const;
		// AudioSource PlaySound(const SoundStream& soundStream, const bool startPaused = false) const;

		void StopAllSounds() const;
		void PauseAllSounds() const;
		void ResumeAllSounds() const;

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
		Listener& GetListener() noexcept { return m_listener; }

	private:
		void Initialise();
		void Destroy();
	};
}

#endif