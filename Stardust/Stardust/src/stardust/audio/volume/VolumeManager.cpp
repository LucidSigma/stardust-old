#include "VolumeManager.h"

void stardust::VolumeManager::AddVolume(const std::string& volumeName, const float value)
{
	if (!m_volumes.contains(volumeName))
	{
		m_volumes.insert({ volumeName, value });
	}
	else
	{
		m_volumes[volumeName] = value;
	}
}

void stardust::VolumeManager::SetVolume(const std::string& volumeName, const float value)
{
	m_volumes[volumeName] = value;
}

[[nodiscard]] float stardust::VolumeManager::GetVolume(const std::string& volumeName) const
{
	return m_volumes.at(volumeName);
}

void stardust::VolumeManager::ResetVolume(const std::string& volumeName)
{
	m_volumes[volumeName] = 1.0f;
}

[[nodiscard]] bool stardust::VolumeManager::DoesVolumeExist(const std::string& volumeName) const
{
	return m_volumes.contains(volumeName);
}

void stardust::VolumeManager::RemoveVolume(const std::string& volumeName)
{
	if (volumeName != s_MasterVolumeName)
	{
		m_volumes.erase(volumeName);
	}
}

void stardust::VolumeManager::ClearAllVolumes()
{
	const float masterVolume = m_volumes[GetMasterVolumeName()];

	m_volumes.clear();
	m_volumes[GetMasterVolumeName()] = masterVolume;
}