#pragma once
#ifndef SOUND_BASE_H
#define SOUND_BASE_H

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <concepts>
#include <cstddef>
#include <string_view>
#include <vector>

#include <soloud/soloud.h>
#undef min
#undef max

#include "../../vfs/VFS.h"
#include "SoundEnums.h"

namespace stardust
{
	template <std::derived_from<SoLoud::AudioSource> T>
	class SoundBase
		: private INoncopyable, private INonmovable
	{
	private:
		T m_handle;
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
		SoundBase(const std::string_view& filepath)
		{
			const std::vector<std::byte> rawSoundData = vfs::ReadFileData(filepath);

			const SoLoud::result loadStatus = m_handle.loadMem(
				reinterpret_cast<const unsigned char*>(rawSoundData.data()),
				static_cast<unsigned int>(rawSoundData.size()),
				true,
				false
			);
			m_isValid = loadStatus == 0;

			if (m_isValid)
			{
				m_length = m_handle.getLength();
			}
		}

		~SoundBase() noexcept = default;

		inline bool IsValid() const noexcept { return m_isValid; }

		void StopAll()
		{
			m_handle.stop();
		}

		double GetLength() const noexcept { return m_length; }

		inline bool IsSingleInstance() const noexcept { return m_isSingleInstance; }

		void SetSingleInstance(const bool isSingleInstance)
		{
			m_handle.setSingleInstance(isSingleInstance);
			m_isSingleInstance = isSingleInstance;
		}

		inline float GetVolume() const noexcept { return m_volume; }
		
		void SetVolume(const float volume)
		{
			m_handle.setVolume(volume);
			m_volume = volume;
		}

		inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }

		void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour)
		{
			switch (inaudibleBehaviour)
			{
			case InaudibleBehaviour::KeepTicking:
				m_handle.setInaudibleBehavior(true, false);

				break;

			case InaudibleBehaviour::Kill:
				m_handle.setInaudibleBehavior(false, true);

				break;

			case InaudibleBehaviour::Pause:
			default:
				m_handle.setInaudibleBehavior(false, false);

				break;
			}

			m_inaudibleBehaviour = inaudibleBehaviour;
		}

		inline bool IsLooping() const noexcept { return m_isLooping; }

		void SetLooping(const bool enableLooping)
		{
			m_handle.setLooping(enableLooping);
			m_isLooping = enableLooping;
		}

		inline double GetLoopPoint() const noexcept { return m_loopPoint; }

		void SetLoopPoint(const double loopPoint)
		{
			m_handle.setLoopPoint(loopPoint);
			m_loopPoint = loopPoint;
		}

		inline float GetDopplerFactor() const noexcept { return m_dopplerFactor; }

		void SetDopplerFactor(const float dopplerFactor)
		{
			m_handle.set3dDopplerFactor(dopplerFactor);
			m_dopplerFactor = dopplerFactor;
		}

		inline AttenuationModel GetAttenuationModel() const noexcept { return m_attenuationModel; }

		void SetAttenuationModel(const AttenuationModel attenuationModel)
		{
			m_handle.set3dAttenuation(static_cast<unsigned int>(attenuationModel), m_attenuationRolloffFactor);
			m_attenuationModel = attenuationModel;
		}

		inline float GetAttenuationRolloffFactor() const noexcept { return m_attenuationRolloffFactor; }

		void SetAttenuationRolloffFactor(const float attenuationRolloffFactor)
		{
			m_handle.set3dAttenuation(static_cast<unsigned int>(m_attenuationModel), attenuationRolloffFactor);
			m_attenuationRolloffFactor = attenuationRolloffFactor;
		}

		inline bool IsDistanceDelayEnabledBy() const noexcept { return m_isDistanceDelayEnabled; }

		void SetDistanceDelayBy(const bool enableDistanceDelay)
		{
			m_handle.set3dDistanceDelay(enableDistanceDelay);
			m_isDistanceDelayEnabled = enableDistanceDelay;
		}

		inline bool IsRelativeToListenerBy() const noexcept { return m_isRelativeToListener; }

		void SetRelativeToListenerBy(const bool isRelativeToListener)
		{
			m_handle.set3dListenerRelative(isRelativeToListener);
			m_isRelativeToListener = isRelativeToListener;
		}

		inline float GetMinDistance() const noexcept { return m_minDistance; }

		void SetMinDistance(const float minDistance)
		{
			m_handle.set3dMinMaxDistance(minDistance, m_maxDistance);
			m_minDistance = minDistance;
		}

		inline float GetMaxDistance() const noexcept { return m_maxDistance; }

		void SetMaxDistance(const float maxDistance)
		{
			m_handle.set3dMinMaxDistance(m_minDistance, maxDistance);
			m_maxDistance = maxDistance;
		}

		inline T& GetRawHandle() noexcept { return m_handle; }
		inline const T& GetRawHandle() const noexcept { return m_handle; }
	};
}

#endif