#include "Camera.h"

namespace stardust
{
    Camera::Camera(const Renderer& renderer, const float halfSize)
    {
        Initialise(renderer, halfSize);
    }

    void Camera::Initialise(const Renderer& renderer, const float halfSize)
    {
        m_halfSize = halfSize;
        m_aspectRatio = renderer.GetLogicalSize().x / renderer.GetLogicalSize().y;
    }

    [[nodiscard]] float Camera::GetPixelsPerUnit(const Window& window) const noexcept
    {
        return window.GetSize().x / (m_halfSize * 2.0f);
    }
}