#include "Renderer.h"

#include <cstdint>
#include <utility>

#include "../../graphics/window/Window.h"
#include "../texture/Texture.h"

namespace stardust
{
	void Renderer::RendererDestroyer::operator()(SDL_Renderer* const renderer) const noexcept
	{
		SDL_DestroyRenderer(renderer);
	}

	[[nodiscard]] unsigned int Renderer::GetRenderDriverCount()
	{
		return static_cast<unsigned int>(SDL_GetNumRenderDrivers());
	}

	Renderer::Renderer()
	{ }

	Renderer::Renderer(const Window& window, const CreateInfo& createInfo)
	{
		Initialise(window, createInfo);
	}

	Renderer::Renderer(Renderer&& other) noexcept
		: m_handle(nullptr), m_window(nullptr), m_presentVSync(false), m_allowRenderToTexture(false), m_currentRenderTarget(nullptr)
	{
		std::swap(m_handle, other.m_handle);
		std::swap(m_window, other.m_window);

		std::swap(m_presentVSync, other.m_presentVSync);
		std::swap(m_allowRenderToTexture, other.m_allowRenderToTexture);

		std::swap(m_currentRenderTarget, other.m_currentRenderTarget);
	}

	Renderer& Renderer::operator =(Renderer&& other) noexcept
	{
		m_handle = std::exchange(other.m_handle, nullptr);
		m_window = std::exchange(other.m_window, nullptr);

		m_presentVSync = std::exchange(other.m_presentVSync, false);
		m_allowRenderToTexture = std::exchange(other.m_allowRenderToTexture, false);

		m_currentRenderTarget = std::exchange(other.m_currentRenderTarget, nullptr);

		return *this;
	}

	Renderer::~Renderer() noexcept
	{
		Destroy();
	}

	void Renderer::Initialise(const Window& window, const CreateInfo& createInfo)
	{
		std::uint32_t rendererFlags = SDL_RENDERER_ACCELERATED;

		if (createInfo.presentVSync)
		{
			rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}

		if (createInfo.allowRenderToTexture)
		{
			rendererFlags |= SDL_RENDERER_TARGETTEXTURE;
		}

		m_handle = std::unique_ptr<SDL_Renderer, RendererDestroyer>(SDL_CreateRenderer(window.GetRawHandle(), -1, rendererFlags));

		if (m_handle != nullptr)
		{
			m_window = &window;

			m_presentVSync = createInfo.presentVSync;
			m_allowRenderToTexture = createInfo.allowRenderToTexture;

			SetBlendMode(createInfo.blendMode);
		}
	}

	void Renderer::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			m_handle = nullptr;

			m_presentVSync = false;
			m_allowRenderToTexture = false;
		}
	}

	[[nodiscard]] rect::Rect Renderer::GetViewport() const
	{
		rect::Rect viewport{ };
		SDL_RenderGetViewport(GetRawHandle(), &viewport);

		return viewport;
	}

	void Renderer::SetViewport(const rect::Rect& viewport) const
	{
		SDL_RenderSetViewport(GetRawHandle(), &viewport);
	}

	void Renderer::ResetViewport() const
	{
		SDL_RenderSetViewport(GetRawHandle(), nullptr);
	}

	[[nodiscard]] bool Renderer::IsClipRectActive() const noexcept
	{
		return static_cast<bool>(SDL_RenderIsClipEnabled(GetRawHandle()));
	}

	[[nodiscard]] rect::Rect Renderer::GetClipRect() const
	{
		if (IsClipRectActive())
		{
			rect::Rect clipRect{ };
			SDL_RenderGetClipRect(GetRawHandle(), &clipRect);

			return clipRect;
		}
		else
		{
			return rect::Rect{ 0u, 0u, 0u, 0u };
		}
	}

	void Renderer::SetClipRect(const rect::Rect& clipRect) const
	{
		SDL_RenderSetClipRect(GetRawHandle(), &clipRect);
	}

	void Renderer::RemoveClipRect() const
	{
		SDL_RenderSetClipRect(GetRawHandle(), nullptr);
	}

	void Renderer::Flush() const
	{
		SDL_RenderFlush(GetRawHandle());
	}

	void Renderer::Clear(const Colour& colour) const
	{
		SetDrawColour(colour);
		SDL_RenderClear(GetRawHandle());
	}

	void Renderer::DrawPoint(const rect::Point& point, const Colour& colour) const
	{
		SDL_RenderDrawPoint(GetRawHandle(), point.x, point.y);
	}

	void Renderer::DrawPoints(const std::vector<rect::Point>& points, const Colour& colour) const
	{
		SDL_RenderDrawPoints(GetRawHandle(), points.data(), static_cast<int>(points.size()));
	}

	void Renderer::DrawLine(const rect::Line& line, const Colour& colour) const
	{
		SetDrawColour(colour);
		SDL_RenderDrawLine(GetRawHandle(), line.first.x, line.first.y, line.second.x, line.second.y);
	}

	void Renderer::DrawConnectedLines(const std::vector<rect::Point>& points, const Colour& colour) const
	{
		SetDrawColour(colour);
		SDL_RenderDrawLines(GetRawHandle(), points.data(), static_cast<int>(points.size()));
	}

	void Renderer::DrawRect(const rect::Rect& rect, const Colour& colour, const RectDrawStyle drawStyle) const
	{
		SetDrawColour(colour);
		
		switch (drawStyle)
		{
		case RectDrawStyle::Outline:
			SDL_RenderDrawRect(GetRawHandle(), &rect);

			break;

		case RectDrawStyle::Filled:
		default:
			SDL_RenderFillRect(GetRawHandle(), &rect);

			break;
		}
	}

	void Renderer::DrawRects(const std::vector<rect::Rect>& rects, const Colour& colour, const RectDrawStyle drawStyle) const
	{
		SetDrawColour(colour);

		switch (drawStyle)
		{
		case RectDrawStyle::Outline:
			SDL_RenderDrawRects(GetRawHandle(), rects.data(), static_cast<int>(rects.size()));

			break;

		case RectDrawStyle::Filled:
		default:
			SDL_RenderFillRects(GetRawHandle(), rects.data(), static_cast<int>(rects.size()));

			break;
		}
	}

	void Renderer::DrawTexture(const Texture& texture, const std::optional<rect::Rect>& sourceRect, const glm::vec2& position, const glm::vec2& scale) const
	{
		const rect::Rect* sourceRectPointer = sourceRect.has_value() ? &sourceRect.value() : nullptr;
		const rect::Rect destinationRect = rect::Create(
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			static_cast<unsigned int>((sourceRect.has_value() ? sourceRect->w : texture.GetSize().x) * scale.x),
			static_cast<unsigned int>((sourceRect.has_value() ? sourceRect->h : texture.GetSize().y) * scale.y)
		);

		SDL_RenderCopy(GetRawHandle(), texture.GetRawHandle(), sourceRectPointer, &destinationRect);
	}

	void Renderer::DrawRotatedTexture(
		const Texture& texture,
		const std::optional<rect::Rect>& sourceRect,
		const glm::vec2& position,
		const glm::vec2& scale,
		const float angle,
		const std::optional<rect::Point>& centre,
		const FlipAxis flipAxis
	) const
	{
		const rect::Rect* sourceRectPointer = sourceRect.has_value() ? &sourceRect.value() : nullptr;
		const rect::Point* centrePointer = centre.has_value() ? &centre.value() : nullptr;
		const rect::Rect destinationRect = rect::Create(
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			static_cast<unsigned int>((sourceRect.has_value() ? sourceRect->w : texture.GetSize().x) * scale.x),
			static_cast<unsigned int>((sourceRect.has_value() ? sourceRect->h : texture.GetSize().y) * scale.y)
		);

		SDL_RenderCopyEx(GetRawHandle(), texture.GetRawHandle(), sourceRectPointer, &destinationRect, angle, centrePointer, static_cast<SDL_RendererFlip>(flipAxis));
	}

	[[nodiscard]] PixelSurface Renderer::ReadPixels(const std::optional<rect::Rect>& areaToRead) const
	{
		unsigned int width = 0u;
		unsigned int height = 0u;

		if (areaToRead.has_value())
		{
			width = static_cast<unsigned int>(areaToRead->w);
			height = static_cast<unsigned int>(areaToRead->h);
		}
		else if (const auto currentRenderTarget = GetCurrentRenderTarget();
				 currentRenderTarget != nullptr)
		{
			width = currentRenderTarget->GetSize().x;
			height = currentRenderTarget->GetSize().y;
		}
		else
		{
			width = m_window->GetSize().x;
			height = m_window->GetSize().y;
		}

		PixelSurface pixelData(width, height);

		if (!pixelData.IsValid())
		{
			return PixelSurface();
		}

		const rect::Rect* readAreaPointer = areaToRead.has_value() ? &areaToRead.value() : nullptr;
		pixelData.Lock();

		if (SDL_RenderReadPixels(GetRawHandle(), readAreaPointer, pixelData.GetFormat(), pixelData.GetPixels(), static_cast<int>(pixelData.GetPitch())) != 0)
		{
			pixelData.Unlock();
			
			return PixelSurface();
		}

		pixelData.Unlock();

		return pixelData;
	}

	void Renderer::SetCurrentRenderTarget(Texture& targetTexture) const
	{
		if (m_allowRenderToTexture)
		{
			if (SDL_SetRenderTarget(GetRawHandle(), targetTexture.GetRawHandle()) == 0)
			{
				m_currentRenderTarget = &targetTexture;
			}
		}
	}

	void Renderer::ResetRenderTarget() const
	{
		if (SDL_SetRenderTarget(GetRawHandle(), nullptr) == 0)
		{
			m_currentRenderTarget = nullptr;
		}
	}

	void Renderer::Present() const
	{
		SDL_RenderPresent(GetRawHandle());
	}

	[[nodiscard]] Renderer::BlendMode Renderer::GetBlendMode() const noexcept
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID;
		SDL_GetRenderDrawBlendMode(GetRawHandle(), &blendMode);

		return static_cast<BlendMode>(blendMode);
	}

	void Renderer::SetBlendMode(const BlendMode blendMode) const noexcept
	{
		SDL_SetRenderDrawBlendMode(GetRawHandle(), static_cast<SDL_BlendMode>(blendMode));
	}

	[[nodiscard]] glm::uvec2 Renderer::GetOutputSize() const noexcept
	{
		int width = 0;
		int height = 0;
		SDL_GetRendererOutputSize(GetRawHandle(), &width, &height);

		return glm::uvec2{ width, height };
	}

	[[nodiscard]] glm::uvec2 Renderer::GetLogicalSize() const noexcept
	{
		int width = 0;
		int height = 0;
		SDL_RenderGetLogicalSize(GetRawHandle(), &width, &height);

		return glm::uvec2{ width, height };
	}

	void Renderer::SetLogicalSize(const glm::uvec2& logicalSize) const noexcept
	{
		SDL_RenderSetLogicalSize(GetRawHandle(), static_cast<int>(logicalSize.x), static_cast<int>(logicalSize.y));
	}

	glm::vec2 Renderer::GetLogicalSizeScale() const noexcept
	{
		return static_cast<glm::vec2>(GetLogicalSize()) / static_cast<glm::vec2>(m_window->GetSize());
	}

	[[nodiscard]] glm::vec2 Renderer::GetScale() const noexcept
	{
		float xScale = 0.0f;
		float yScale = 0.0f;
		SDL_RenderGetScale(GetRawHandle(), &xScale, &yScale);

		return glm::vec2{ xScale, yScale };
	}

	void Renderer::SetScale(const glm::vec2& scale) const noexcept
	{
		SDL_RenderSetScale(GetRawHandle(), scale.x, scale.y);
	}

	[[nodiscard]] bool Renderer::IsIntegerScalingEnabled() const noexcept
	{
		return static_cast<bool>(SDL_RenderGetIntegerScale(GetRawHandle()));
	}

	void Renderer::SetIntegerScaling(const bool enableIntegerScaling) const noexcept
	{
		SDL_RenderSetIntegerScale(GetRawHandle(), static_cast<SDL_bool>(enableIntegerScaling));
	}

	void Renderer::SetDrawColour(const Colour& colour) const
	{
		SDL_SetRenderDrawColor(GetRawHandle(), colour.r, colour.g, colour.b, colour.a);
	}
}