#pragma once
#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include "../utility/interfaces/INoncopyable.h"

#include <limits>

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
		class SoundSystem* m_soundSystem = nullptr;
		SoLoud::handle m_handle = std::numeric_limits<SoLoud::handle>::max();

		glm::vec3 m_position = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_velocity = glm::vec3{ 0.0f, 0.0f, 0.0f };

		float m_minDistance = 1.0f;
		float m_maxDistance = 1'000'000.0f;

		InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::Pause;

	public:
		SoundSource() = default;
		SoundSource(const SoLoud::handle handle, class SoundSystem& soundSystem, const glm::vec3& position = glm::vec3{ 0.0f, 0.0f, 0.0f }, const glm::vec3& velocity = glm::vec3{ 0.0f, 0.0f, 0.0f });
		SoundSource(SoundSource&& other) noexcept;
		SoundSource& operator =(SoundSource&& other) noexcept;
		~SoundSource() noexcept = default;

		inline bool HasValidHandle() const noexcept { return m_soundSystem != nullptr; }

		void Pause() const;
		void Resume() const;
		void Stop() const;

		[[nodiscard]] bool IsPlaying() const;
		[[nodiscard]] bool IsPaused() const;
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
		void Rewind();

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

		inline const glm::vec3& GetPosition() const noexcept { return m_position; }
		void SetPosition(const glm::vec3& position);
		inline const glm::vec3& GetVelocity() const noexcept { return m_velocity; }
		void SetVelocity(const glm::vec3& velocity);

		inline float GetMinDistance() const noexcept { return m_minDistance; }
		void SetMinDistance(const float minDistance);
		inline float GetMaxDistance() const noexcept { return m_maxDistance; }
		void SetMaxDistance(const float maxDistance);

		inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }
		void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour);

		[[nodiscard]] bool IsProtected() const;
		void SetProtection(const bool isProtected);

		inline SoLoud::handle GetRawHandle() const noexcept { return m_handle; }
	};
}

#endif