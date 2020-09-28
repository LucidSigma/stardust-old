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

		float m_defaultVolume = 1.0f;
		InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::Pause;
		
		bool m_isLooping = false;
		double m_defaultLoopPoint = 0.0f;

		float m_defaultDopplerFactor = 1.0f;
		AttenuationModel m_defaultAttenuationModel = AttenuationModel::None;
		float m_defaultAttenuationRolloffFactor = 1.0f;
		
		bool m_distanceDelayByDefault = false;
		bool m_relativeToListenerByDefault = false;

		float m_defaultMinDistance = 1.0f;
		float m_defaultMaxDistance = 1'000'000.0f;

	public:
		Sound(const std::string_view& filepath);
		~Sound() noexcept = default;

		inline bool IsValid() const noexcept { return m_isValid; }

		void StopAll();

		double GetLength() const noexcept { return m_length; }

		bool IsSingleInstance() const noexcept { return m_isSingleInstance; }
		void SetSingleInstance(const bool isSingleInstance);

		inline float GetDefaultVolume() const noexcept { return m_defaultVolume; }
		void SetDefaultVolume(const float defaultVolume);

		inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }
		void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour);

		bool IsLoopingByDefault() const noexcept { return m_isLooping; }
		void SetLoopingByDefault(const bool loopByDefault);
		double GetDefaultLoopPoint() const noexcept { return m_defaultLoopPoint; }
		void SetDefaultLoopPoint(const double defaultLoopPoint);

		float GetDefaultDopplerFactor() const noexcept { return m_defaultDopplerFactor; }
		void SetDefaultDopplerFactor(const float defaultDopplerFactor);

		AttenuationModel GetDefaultAttenuationModel() const noexcept { return m_defaultAttenuationModel; }
		void SetDefaultAttenuationModel(const AttenuationModel defaultAttenuationModel);
		float GetDefaultAttenuationRolloffFactor() const noexcept { return m_defaultAttenuationRolloffFactor; }
		void SetDefaultAttenuationRolloffFactor(const float defaultAttenuationRolloffFactor);

		bool IsDistanceDelayEnabledByDefault() const noexcept { return m_distanceDelayByDefault; }
		void SetDistanceDelayByDefault(const bool enableDistanceDelay);

		bool IsRelativeToListenerByDefault() const noexcept { return m_relativeToListenerByDefault; }
		void SetRelativeToListenerByDefault(const bool isRelativeToListener);

		float GetDefaultMinDistance() const noexcept { return m_defaultMinDistance; }
		void SetDefaultMinDistance(const float defaultMinDistance);

		float GetDefaultMaxDistance() const noexcept { return m_defaultMaxDistance; }
		void SetDefaultMaxDistance(const float defaultMaxDistance);

		inline SoLoud::Wav& GetRawHandle() noexcept { return m_handle; }
		inline const SoLoud::Wav& GetRawHandle() const noexcept { return m_handle; }
	};
}

#endif