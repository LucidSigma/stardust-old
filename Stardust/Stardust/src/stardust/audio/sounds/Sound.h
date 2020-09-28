#pragma once
#ifndef SOUND_H
#define SOUND_H

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <string_view>
#include <type_traits>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#undef min
#undef max

namespace stardust
{
	class Sound
		: private INoncopyable, private INonmovable
	{
	public:
		enum class InaudibleBehaviour
		{
			Pause,
			KeepTicking,
			Kill,
		};

		enum class AttenuationModel
			: std::underlying_type_t<SoLoud::AudioSource::ATTENUATION_MODELS>
		{
			None = SoLoud::AudioSource::NO_ATTENUATION,
			InverseDistance = SoLoud::AudioSource::INVERSE_DISTANCE,
			LinearDistance = SoLoud::AudioSource::LINEAR_DISTANCE,
			ExponentialDistance = SoLoud::AudioSource::EXPONENTIAL_DISTANCE,
		};

	private:
		SoLoud::Wav m_handle;
		bool m_isValid = false;

		double m_length = 0.0f;
		bool m_isSingleInstance = false;

		float m_volume = 1.0f;
		InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::Pause;
		
		bool m_isLooping = false;
		double m_loopPoint = 0.0f;

		float m_dopplerFactor = 1.0f;
		AttenuationModel m_attenuationModel = AttenuationModel::None;
		float m_attenuationRolloffFactor = 1.0f;
		
		bool m_isDistanceDelayEnabled = false;
		bool m_isRelativeToListener = false;

		float m_minDistance = 1.0f;
		float m_maxDistance = 1'000'000.0f;

	public:
		Sound(const std::string_view& filepath);
		~Sound() noexcept = default;

		inline bool IsValid() const noexcept { return m_isValid; }

		void StopAll();

		double GetLength() const noexcept { return m_length; }

		bool IsSingleInstance() const noexcept { return m_isSingleInstance; }
		void SetSingleInstance(const bool isSingleInstance);

		inline float GetVolume() const noexcept { return m_volume; }
		void SetVolume(const float volume);

		inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }
		void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour);

		bool IsLooping() const noexcept { return m_isLooping; }
		void SetLooping(const bool enableLooping);
		double GetLoopPoint() const noexcept { return m_loopPoint; }
		void SetLoopPoint(const double loopPoint);

		float GetDopplerFactor() const noexcept { return m_dopplerFactor; }
		void SetDopplerFactor(const float dopplerFactor);

		AttenuationModel GetAttenuationModel() const noexcept { return m_attenuationModel; }
		void SetAttenuationModel(const AttenuationModel attenuationModel);
		float GetAttenuationRolloffFactor() const noexcept { return m_attenuationRolloffFactor; }
		void SetAttenuationRolloffFactor(const float attenuationRolloffFactor);

		bool IsDistanceDelayEnabledBy() const noexcept { return m_isDistanceDelayEnabled; }
		void SetDistanceDelayBy(const bool enableDistanceDelay);

		bool IsRelativeToListenerBy() const noexcept { return m_isRelativeToListener; }
		void SetRelativeToListenerBy(const bool isRelativeToListener);

		float GetMinDistance() const noexcept { return m_minDistance; }
		void SetMinDistance(const float minDistance);

		float GetMaxDistance() const noexcept { return m_maxDistance; }
		void SetMaxDistance(const float maxDistance);

		inline SoLoud::Wav& GetRawHandle() noexcept { return m_handle; }
		inline const SoLoud::Wav& GetRawHandle() const noexcept { return m_handle; }
	};
}

#endif