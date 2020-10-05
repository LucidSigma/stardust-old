#include "Camera.h"

#include "../graphics/window/Window.h"

namespace stardust
{
    Camera::Camera(const Renderer& renderer, const float halfSize)
    {
        Initialise(renderer, halfSize);
    }

    void Camera::Initialise(const Renderer& renderer, const float halfSize)
    {
        m_renderer = &renderer;

        m_halfSize = halfSize;
        m_aspectRatio = static_cast<float>(renderer.GetLogicalSize().x) / static_cast<float>(renderer.GetLogicalSize().y);
        m_pixelsPerUnit = renderer.GetLogicalSize().x / (m_halfSize * 2.0f);
    }

    void Camera::Refresh()
    {
        m_pixelsPerUnit = m_renderer->GetLogicalSize().x / (m_halfSize * 2.0f);
    }

    void Camera::SetHalfSize(const float halfSize) noexcept
    {
        m_halfSize = halfSize;
        m_pixelsPerUnit = m_renderer->GetLogicalSize().x / (m_halfSize * 2.0f);
    }

    [[nodiscard]] glm::uvec2 Camera::WorldSpaceToScreenSpace(glm::vec2 position) const noexcept
    {
        position.x += m_halfSize;
        position.y -= m_halfSize / m_aspectRatio;

        position *= m_pixelsPerUnit;

        return glm::uvec2{ position.x, -position.y };
    }

    [[nodiscard]] glm::vec2 Camera::ScreenSpaceToWorldSpace(const glm::uvec2& position) const noexcept
    {
        glm::vec2 worldSpace = position;
        worldSpace.y *= -1.0f;

        worldSpace /= m_pixelsPerUnit;

        worldSpace.x -= m_halfSize;
        worldSpace.y += m_halfSize / m_aspectRatio;

        return worldSpace;
    }
}