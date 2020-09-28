#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <memory>

#include <glm/glm.hpp>
#include <soloud/soloud.h>
#undef min
#undef max

#include "Listener.h"
#include "sounds/Sounds.h"
#include "SoundSource.h"

namespace stardust
{
	class SoundSystem
		: private INoncopyable, private INonmovable
	{
	private:
		std::unique_ptr<SoLoud::Soloud> m_soLoudHandle;
		Listener m_listener;

		bool m_didInitialiseSuccessfully = false;

	public:
		SoundSystem();
		~SoundSystem();

		void Update() const;

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		SoundSource PlaySound(Sound& sound, const bool startPaused = false);
		SoundSource PlaySound(SoundStream& soundStream, const bool startPaused = false);
		SoundSource Play3DSound(Sound& sound, const glm::vec3& position, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f }, const bool startPaused = false);
		SoundSource Play3DSound(SoundStream& soundStream, const glm::vec3& position, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f }, const bool startPaused = false);

		SoundSource PlaySoundWithDelay(Sound& sound, const float secondsDelay);
		SoundSource PlaySoundWithDelay(SoundStream& soundStream, const float secondsDelay);
		SoundSource Play3DSoundWithDelay(Sound& sound, const float secondsDelay, const glm::vec3& position, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f });
		SoundSource Play3DSoundWithDelay(SoundStream& soundStream, const float secondsDelay, const glm::vec3& position, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f });

		SoundSource PlaySoundInBackground(Sound& sound, const bool startPaused = false);
		SoundSource PlaySoundInBackground(SoundStream& soundStream, const bool startPaused = false);

		void PauseAllSounds() const;
		void ResumeAllSounds() const;
		void StopAllSounds() const;

		void FadeVolumeGlobal(const float volumeToFadeTo, const float seconds) const;

		void OscillateGlobalVolume(const float toVolume, const float fromVolume, const float frequency) const;
		void ResetGlobalVolumeOscillation() const;

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