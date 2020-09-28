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
			true,
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

	void Sound::SetDefaultVolume(const float defaultVolume)
	{
		m_handle.setVolume(defaultVolume);
		m_defaultVolume = defaultVolume;
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

	void Sound::SetLoopingByDefault(const bool loopByDefault)
	{
		m_handle.setLooping(loopByDefault);
		m_isLooping = loopByDefault;
	}

	void Sound::SetDefaultLoopPoint(const double defaultLoopPoint)
	{
		m_handle.setLoopPoint(defaultLoopPoint);
		m_defaultLoopPoint = defaultLoopPoint;
	}

	void Sound::SetDefaultDopplerFactor(const float defaultDopplerFactor)
	{
		m_handle.set3dDopplerFactor(defaultDopplerFactor);
		m_defaultDopplerFactor = defaultDopplerFactor;
	}

	void Sound::SetDefaultAttenuationModel(const AttenuationModel defaultAttenuationModel)
	{
		m_handle.set3dAttenuation(static_cast<unsigned int>(defaultAttenuationModel), m_defaultAttenuationRolloffFactor);
		m_defaultAttenuationModel = defaultAttenuationModel;
	}

	void Sound::SetDefaultAttenuationRolloffFactor(const float defaultAttenuationRolloffFactor)
	{
		m_handle.set3dAttenuation(static_cast<unsigned int>(m_defaultAttenuationModel), defaultAttenuationRolloffFactor);
		m_defaultAttenuationRolloffFactor = defaultAttenuationRolloffFactor;
	}

	void Sound::SetDistanceDelayByDefault(const bool enableDistanceDelay)
	{
		m_handle.set3dDistanceDelay(enableDistanceDelay);
		m_distanceDelayByDefault = enableDistanceDelay;
	}

	void Sound::SetRelativeToListenerByDefault(const bool isRelativeToListener)
	{
		m_handle.set3dListenerRelative(isRelativeToListener);
		m_relativeToListenerByDefault = isRelativeToListener;
	}

	void Sound::SetDefaultMinDistance(const float defaultMinDistance)
	{
		m_handle.set3dMinMaxDistance(defaultMinDistance, m_defaultMaxDistance);
		m_defaultMinDistance = defaultMinDistance;
	}

	void Sound::SetDefaultMaxDistance(const float defaultMaxDistance)
	{
		m_handle.set3dMinMaxDistance(m_defaultMinDistance, defaultMaxDistance);
		m_defaultMaxDistance = defaultMaxDistance;
	}
}