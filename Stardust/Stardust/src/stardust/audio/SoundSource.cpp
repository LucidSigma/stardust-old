#include "SoundSource.h"

#include <algorithm>

#include "SoundSystem.h"

namespace stardust
{
	SoundSource::SoundSource(const SoLoud::handle handle, SoundSystem& soundSystem, const glm::vec3& position, const glm::vec3& velocity)
		: m_soundSystem(&soundSystem), m_handle(handle), m_position(position), m_velocity(velocity)
	{ }

	[[nodiscard]] bool SoundSource::IsStopped() const
	{
		return m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle);
	}

	[[nodiscard]] unsigned int SoundSource::GetLoopCount() const
	{
		return m_soundSystem->GetRawHandle().getLoopCount(m_handle);
	}

	[[nodiscard]] double SoundSource::GetStreamTime() const
	{
		return m_soundSystem->GetRawHandle().getStreamTime(m_handle);
	}

	void SoundSource::FadeVolume(const float toVolume, const float fadeTime)
	{
		m_soundSystem->GetRawHandle().fadeVolume(m_handle, toVolume, fadeTime);
	}

	void SoundSource::FadeRelativePlaySpeed(const float toRelativePlaySpeed, const float fadeTime)
	{
		m_soundSystem->GetRawHandle().fadeRelativePlaySpeed(m_handle, toRelativePlaySpeed, fadeTime);
	}

	void SoundSource::FadePan(const float toPan, const float fadeTime)
	{
		m_soundSystem->GetRawHandle().fadePan(m_handle, std::clamp(toPan, -1.0f, 1.0f), fadeTime);
	}

	void SoundSource::SetPan(const float pan)
	{
		m_soundSystem->GetRawHandle().setPan(m_handle, std::clamp(pan, -1.0f, 1.0f));
	}

	void SoundSource::SetPanAbsolute(const float leftVolume, const float rightVolume)
	{
		m_soundSystem->GetRawHandle().setPanAbsolute(m_handle, leftVolume, rightVolume);
	}

	void SoundSource::ResetVolumeOscillation()
	{
		m_soundSystem->GetRawHandle().oscillateVolume(m_handle, 1.0f, 1.0f, 0.0);
	}

	void SoundSource::ResetPanOscillation()
	{
		m_soundSystem->GetRawHandle().oscillatePan(m_handle, 0.0f, 0.0f, 0.0);
	}

	void SoundSource::ResetRelativePlaySpeedOscillation()
	{
		m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, 1.0f, 1.0f, 0.0);
	}

	void SoundSource::SetSampleRate(const float sampleRate)
	{
		m_soundSystem->GetRawHandle().setSamplerate(m_handle, sampleRate);
	}

	void SoundSource::SetProtection(const bool isProtected)
	{
		m_soundSystem->GetRawHandle().setProtectVoice(m_handle, isProtected);
	}
}