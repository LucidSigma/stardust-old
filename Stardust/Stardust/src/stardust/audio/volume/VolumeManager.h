#pragma once
#ifndef VOLUME_MANAGER_H
#define VOLUME_MANAGER_H

#include <unordered_map>
#include <string>
#include <string_view>

namespace stardust
{
	class VolumeManager
	{
	private:
		static constexpr std::string_view s_MasterVolumeName = "master";

		std::unordered_map<std::string, float> m_volumes{ };

	public:
		[[nodiscard]] inline static std::string GetMasterVolumeName() noexcept { return std::string(s_MasterVolumeName); }

		VolumeManager() = default;
		~VolumeManager() noexcept = default;

		void AddVolume(const std::string& volumeName, const float value = 1.0f);
		void SetVolume(const std::string& volumeName, const float value);
		[[nodiscard]] float GetVolume(const std::string& volumeName) const;
		void ResetVolume(const std::string& volumeName);

		[[nodiscard]] bool DoesVolumeExist(const std::string& volumeName) const;

		void RemoveVolume(const std::string& volumeName);
		void ClearAllVolumes();

		inline std::unordered_map<std::string, float>& GetVolumes() noexcept { return m_volumes; }
		inline const std::unordered_map<std::string, float>& GetVolumes() const noexcept { return m_volumes; }
	};
}

#endif