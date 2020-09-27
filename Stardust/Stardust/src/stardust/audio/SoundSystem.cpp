#include "SoundSystem.h"

#include <algorithm>

namespace stardust
{
	SoundSystem::SoundSystem()
		: m_soLoudHandle(std::make_unique<SoLoud::Soloud>()), m_listener(*this)
	{
		Initialise();
	}

	SoundSystem::~SoundSystem()
	{
		Destroy();
	}

	void SoundSystem::Update() const
	{
		m_soLoudHandle->update3dAudio();
	}

	void SoundSystem::StopAllSounds() const
	{
		m_soLoudHandle->stopAll();
	}

	void SoundSystem::FadeVolumeGlobal(const float volumeToFadeTo, const float seconds) const
	{
		m_soLoudHandle->fadeGlobalVolume(volumeToFadeTo, seconds);
	}

	void SoundSystem::OscillateGlobalVolume(const float toVolume, const float fromVolume, const float frequency) const
	{
		m_soLoudHandle->oscillateGlobalVolume(fromVolume, toVolume, frequency);
	}

	void SoundSystem::ResetGlobalOscillation() const
	{
		m_soLoudHandle->oscillateGlobalVolume(1.0f, 1.0f, 0.0f);
	}

	unsigned int SoundSystem::GetPlayingSoundCount() const
	{
		return m_soLoudHandle->getActiveVoiceCount();
	}

	bool SoundSystem::IsSoundPlaying() const
	{
		return m_soLoudHandle->getActiveVoiceCount() > 0u;
	}

	[[nodiscard]] float SoundSystem::GetGlobalVolume() const noexcept
	{
		return m_soLoudHandle->getGlobalVolume();
	}

	void SoundSystem::SetGlobalVolume(const float globalVolume) const noexcept
	{
		m_soLoudHandle->setGlobalVolume(std::clamp(globalVolume, 0.0f, 1.0f));
	}

	[[nodiscard]] float SoundSystem::GetPostClipScaler() const noexcept
	{
		return m_soLoudHandle->getPostClipScaler();
	}

	void SoundSystem::SetPostClipScaler(const float postClipScaler) const noexcept
	{
		m_soLoudHandle->setPostClipScaler(std::clamp(postClipScaler, 0.0f, 1.0f));
	}

	[[nodiscard]] float SoundSystem::GetSpeedOfSound() const noexcept
	{
		return m_soLoudHandle->get3dSoundSpeed();
	}

	void SoundSystem::SetSpeedOfSound(const float speedOfSound) const noexcept
	{
		m_soLoudHandle->set3dSoundSpeed(speedOfSound);
	}

	void SoundSystem::Initialise()
	{
		m_soLoudHandle->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D);
	}

	void SoundSystem::Destroy()
	{
		if (m_soLoudHandle != nullptr)
		{
			m_soLoudHandle->deinit();
			m_soLoudHandle = nullptr;
		}
	}
}