#include "Sound.h"

#include <cstddef>
#include <vector>

#include <soloud/soloud_file.h>

#include "../../vfs/VFS.h"

namespace stardust
{
	Sound::Sound(const std::string_view& filepath)
	{
		const std::vector<std::byte> rawSoundData = vfs::ReadFileData(filepath);

		const SoLoud::result loadStatus = m_handle.loadMem(
			reinterpret_cast<const unsigned char*>(rawSoundData.data()),
			static_cast<unsigned int>(rawSoundData.size()),
			false,
			false
		);
		m_isValid = loadStatus == 0;

		if (m_isValid)
		{
			m_length = m_handle.getLength();
		}
	}

	void Sound::StopAll()
	{
		m_handle.stop();
	}

	void Sound::SetSingleInstance(const bool isSingleInstance)
	{
		m_handle.setSingleInstance(isSingleInstance);
		m_isSingleInstance = isSingleInstance;
	}

	void Sound::SetVolume(const float Volume)
	{
		m_handle.setVolume(Volume);
		m_volume = Volume;
	}

	void Sound::SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour)
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

	void Sound::SetLooping(const bool enableLooping)
	{
		m_handle.setLooping(enableLooping);
		m_isLooping = enableLooping;
	}

	void Sound::SetLoopPoint(const double loopPoint)
	{
		m_handle.setLoopPoint(loopPoint);
		m_loopPoint = loopPoint;
	}

	void Sound::SetDopplerFactor(const float dopplerFactor)
	{
		m_handle.set3dDopplerFactor(dopplerFactor);
		m_dopplerFactor = dopplerFactor;
	}

	void Sound::SetAttenuationModel(const AttenuationModel attenuationModel)
	{
		m_handle.set3dAttenuation(static_cast<unsigned int>(attenuationModel), m_attenuationRolloffFactor);
		m_attenuationModel = attenuationModel;
	}

	void Sound::SetAttenuationRolloffFactor(const float attenuationRolloffFactor)
	{
		m_handle.set3dAttenuation(static_cast<unsigned int>(m_attenuationModel), attenuationRolloffFactor);
		m_attenuationRolloffFactor = attenuationRolloffFactor;
	}

	void Sound::SetDistanceDelayBy(const bool enableDistanceDelay)
	{
		m_handle.set3dDistanceDelay(enableDistanceDelay);
		m_isDistanceDelayEnabled = enableDistanceDelay;
	}

	void Sound::SetRelativeToListenerBy(const bool isRelativeToListener)
	{
		m_handle.set3dListenerRelative(isRelativeToListener);
		m_isRelativeToListener = isRelativeToListener;
	}

	void Sound::SetMinDistance(const float minDistance)
	{
		m_handle.set3dMinMaxDistance(minDistance, m_maxDistance);
		m_minDistance = minDistance;
	}

	void Sound::SetMaxDistance(const float maxDistance)
	{
		m_handle.set3dMinMaxDistance(m_minDistance, maxDistance);
		m_maxDistance = maxDistance;
	}
}