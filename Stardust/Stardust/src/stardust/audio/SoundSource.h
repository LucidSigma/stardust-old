#pragma once
#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include "../utility/interfaces/INoncopyable.h"

#include <glm/glm.hpp>
#include <soloud/soloud.h>
#undef min
#undef max

#include "sounds/SoundEnums.h"

namespace stardust
{
	class SoundSource
		: private INoncopyable
	{
	private:
		class SoundSystem* m_soundSystem;
		SoLoud::handle m_handle;

		glm::vec3 m_position;
		glm::vec3 m_velocity;

		InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::Pause;

	public:
		explicit SoundSource(const SoLoud::handle handle, class SoundSystem& soundSystem, const glm::vec3& position = glm::vec3{ 0.0f, 0.0f, 0.0f }, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f });
		SoundSource(SoundSource&& other) noexcept;
		SoundSource& operator =(SoundSource&& other) noexcept;
		~SoundSource() noexcept = default;

		void Pause() const;
		void Resume() const;
		void Stop() const;

		bool IsPlaying() const;
		bool IsPaused() const;
		[[nodiscard]] bool IsStopped() const;

		void SchedulePause(const float secondsDelay);
		void ScheduleStop(const float secondsDelay);

		[[nodiscard]] bool IsLooping() const;
		void SetLooping(const bool isLooping);
		[[nodiscard]] float GetLoopPoint() const;
		void SetLoopPoint(const float loopPoint);
		[[nodiscard]] unsigned int GetLoopCount() const;

		[[nodiscard]] double GetStreamTime() const;
		void Seek(const double seconds);

		[[nodiscard]] float GetVolume() const;
		[[nodiscard]] float GetOverallVolume() const;
		void SetVolume(const float volume);

		[[nodiscard]] float GetPan() const;
		void SetPan(const float pan);
		void SetPanAbsolute(const float leftVolume, const float rightVolume);

		[[nodiscard]] float GetRelativePlaySpeed() const;
		void SetRelativePlaySpeed(const float relativePlaySpeed);
		[[nodiscard]] float GetSampleRate() const;
		void SetSampleRate(const float sampleRate);

		void FadeVolume(const float toVolume, const float fadeTime);
		void FadePan(const float toPan, const float fadeTime);
		void FadeRelativePlaySpeed(const float toRelativePlaySpeed, const float fadeTime);

		void OscillateVolume(const float toVolume, const float fromVolume, const float frequency);
		void ResetVolumeOscillation();
		void OscillatePan(const float toPan, const float fromPan, const float frequency);
		void ResetPanOscillation();
		void OscillateRelativePlaySpeed(const float toRelativePlaySpeed, const float fromRelativePlaySpeed, const float frequency);
		void ResetRelativePlaySpeedOscillation();

		const glm::vec3& GetPosition() const noexcept;
		void SetPosition(const glm::vec3& position);
		const glm::vec3& GetVelocity() const noexcept;
		void SetVelocity(const glm::vec3& velocity);

		float GetMinDistance() const noexcept;
		void SetMinDistance(const float minDistance);
		float GetMaxDistance() const noexcept;
		void SetMaxDistance(const float maxDistance);

		inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }
		void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour);

		[[nodiscard]] bool IsProtected() const;
		void SetProtection(const bool isProtected);

		[[nodiscard]] float GetPostClipScaler() const;
		void SetPostClipScaler(const float postClipScaler);

		inline SoLoud::handle GetRawHandle() const noexcept { return m_handle; }
	};
}

#endif