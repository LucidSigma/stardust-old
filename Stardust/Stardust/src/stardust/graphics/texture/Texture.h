#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../utility/interfaces/INoncopyable.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../../rect/Rect.h"
#include "../renderer/Renderer.h"

namespace stardust
{
	class Texture
		: private INoncopyable
	{
	public:
		enum class AccessType
			: std::underlying_type_t<SDL_TextureAccess>
		{
			Static = SDL_TEXTUREACCESS_STATIC,
			Streaming = SDL_TEXTUREACCESS_STREAMING,
			Target = SDL_TEXTUREACCESS_TARGET,
		};

		enum class ScaleMode
			: std::underlying_type_t<SDL_ScaleMode>
		{
			Nearest = SDL_ScaleModeNearest,
			Linear = SDL_ScaleModeLinear,
			Anisotropic = SDL_ScaleModeBest,
		};

	private:
		struct TextureDestroyer
		{
			void operator ()(SDL_Texture* const texture) const noexcept;
		};

		const Renderer* m_renderer = nullptr;
		std::unique_ptr<SDL_Texture, TextureDestroyer> m_handle = nullptr;

		glm::uvec2 m_size{ 0u, 0u };
		std::uint32_t m_format = 0u;
		AccessType m_accessType = AccessType::Static;

		bool m_isLocked = false;

	public:
		Texture() = default;
		Texture(const Renderer& renderer, const std::string_view& filepath);
		Texture(const Renderer& renderer, const std::uint32_t format, const AccessType accessType, const glm::uvec2& size);

		Texture(Texture&& other) noexcept;
		Texture& operator =(Texture&& other) noexcept;

		~Texture() noexcept;

		void Initialise(const Renderer& renderer, const std::string_view& filepath);
		void Initialise(const Renderer& renderer, const std::uint32_t format, const AccessType accessType, const glm::uvec2& size);
		void Destroy() noexcept;

		void Lock(const std::optional<rect::Rect>& lockingArea, void*& out_pixels, unsigned int& out_pitch);
		void Unlock();
		inline bool IsLocked() const noexcept { return m_isLocked; }

		void UpdateArea(const std::optional<rect::Rect>& areaToUpdate, const void* const pixels, const unsigned int pixelRowLength);

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		[[nodiscard]] inline SDL_Texture* const GetRawHandle() const noexcept { return m_handle.get(); }

		inline const glm::uvec2& GetSize() const noexcept { return m_size; }
		inline std::uint32_t GetFormat() const noexcept { return m_format; }
		inline AccessType GetAccessType() const noexcept { return m_accessType; }

		[[nodiscard]] ScaleMode GetScaleMode() const noexcept;
		void SetScaleMode(const ScaleMode scaleMode) const noexcept;

		[[nodiscard]] Renderer::BlendMode GetBlendMode() const noexcept;
		void SetBlendMode(const Renderer::BlendMode blendMode) const noexcept;

		[[nodiscard]] std::uint8_t GetAlphaMod() const noexcept;
		void SetAlphaMod(const std::uint8_t alphaMod) const noexcept;

		[[nodiscard]] std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> GetColourMod() const noexcept;
		void SetColourMod(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue) const noexcept;
	};
}

#endif