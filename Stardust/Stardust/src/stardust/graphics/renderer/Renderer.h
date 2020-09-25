#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "../../utility/interfaces/INoncopyable.h"

#include <memory>
#include <optional>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../../rect/Rect.h"
#include "../Colour.h"

namespace stardust
{
	class Renderer
		: private INoncopyable
	{
	public:
		enum class RectDrawStyle
		{
			Outline,
			Filled,
		};

		enum class FlipAxis
			: std::underlying_type_t<SDL_RendererFlip>
		{
			None = SDL_FLIP_NONE,
			Horizontal = SDL_FLIP_HORIZONTAL,
			Vertical = SDL_FLIP_VERTICAL,
			Both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL,
		};

		enum class BlendMode
			: std::underlying_type_t<SDL_BlendMode>
		{
			None = SDL_BLENDMODE_NONE,
			Alpha = SDL_BLENDMODE_BLEND,
			Additive = SDL_BLENDMODE_ADD,
			Modulate = SDL_BLENDMODE_MOD,
			Multiply = SDL_BLENDMODE_MUL,
		};

		struct CreateInfo
		{
			bool presentVSync;
			bool allowRenderToTexture;

			BlendMode blendMode;
		};

	private:
		struct RendererDestroyer
		{
			void operator ()(SDL_Renderer* const renderer) const noexcept;
		};

		std::unique_ptr<SDL_Renderer, RendererDestroyer> m_handle = nullptr;
		const class Window* m_window = nullptr;

		bool m_presentVSync = false;
		bool m_allowRenderToTexture = false;

		mutable class Texture* m_currentRenderTarget = nullptr;

	public:
		[[nodiscard]] static unsigned int GetRenderDriverCount();

		Renderer();
		explicit Renderer(const class Window& window, const CreateInfo& createInfo);

		Renderer(Renderer&& other) noexcept;
		Renderer& operator =(Renderer&& other) noexcept;

		~Renderer() noexcept;

		void Initialise(const class Window& window, const CreateInfo& createInfo);
		void Destroy() noexcept;

		[[nodiscard]] rect::Rect GetViewport() const;
		void SetViewport(const rect::Rect& viewport) const;
		void ResetViewport() const;

		[[nodiscard]] bool IsClipRectActive() const noexcept;
		[[nodiscard]] rect::Rect GetClipRect() const;
		void SetClipRect(const rect::Rect& clipRect) const;
		void RemoveClipRect() const;

		void Flush() const;
		void Clear(const Colour& colour) const;

		void DrawPoint(const rect::Point& point, const Colour& colour) const;
		void DrawPoints(const std::vector<rect::Point>& points, const Colour& colour) const;
		void DrawLine(const rect::Line& line, const Colour& colour) const;
		void DrawConnectedLines(const std::vector<rect::Point>& points, const Colour& colour) const;
		void DrawRect(const rect::Rect& rect, const Colour& colour, const RectDrawStyle drawStyle = RectDrawStyle::Filled) const;
		void DrawRects(const std::vector<rect::Rect>& rects, const Colour& colour, const RectDrawStyle drawStyle = RectDrawStyle::Filled) const;

		void DrawTexture(const Texture& texture, const std::optional<rect::Rect>& sourceRect, const glm::vec2& position, const glm::vec2& scale) const;
		void DrawRotatedTexture(
			const class Texture& texture,
			const std::optional<rect::Rect>& sourceRect,
			const glm::vec2& position,
			const glm::vec2& scale,
			const float angle,
			const std::optional<rect::Point>& centre,
			const FlipAxis flipAxis = FlipAxis::None
		) const;

		SDL_Surface* ReadPixels(const std::optional<rect::Rect>& areaToRead = std::nullopt) const;

		inline class Texture* GetCurrentRenderTarget() const noexcept { return m_currentRenderTarget; }
		void SetCurrentRenderTarget(class Texture& targetTexture) const;
		void ResetRenderTarget() const;

		void Present() const;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		[[nodiscard]] inline SDL_Renderer* const GetRawHandle() const noexcept { return m_handle.get(); }
		[[nodiscard]] inline const class Window& GetWindow() const noexcept { return *m_window; }
		
		inline bool IsPresentingVSync() const noexcept { return m_presentVSync; }
		inline bool CanRenderToTexture() const noexcept { return m_allowRenderToTexture; }

		[[nodiscard]] BlendMode GetBlendMode() const noexcept;
		void SetBlendMode(const BlendMode blendMode) const noexcept;

		[[nodiscard]] glm::uvec2 GetOutputSize() const noexcept;

		[[nodiscard]] glm::uvec2 GetLogicalSize() const noexcept;
		void SetLogicalSize(const glm::uvec2& logicalSize) const noexcept;

		[[nodiscard]] glm::vec2 GetScale() const noexcept;
		void SetScale(const glm::vec2& scale) const noexcept;
		[[nodiscard]] bool IsIntegerScalingEnabled() const noexcept;
		void SetIntegerScaling(const bool enableIntegerScaling) const noexcept;

	private:
		void SetDrawColour(const Colour& colour) const;
	};
}

#endif