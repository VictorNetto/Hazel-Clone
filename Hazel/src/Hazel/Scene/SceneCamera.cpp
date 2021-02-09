#include "Hazel/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

    SceneCamera::SceneCamera()
    {
        RecalculateProjectio();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;

        RecalculateProjectio();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;

        RecalculateProjectio();
    }

    void SceneCamera::RecalculateProjectio()
    {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRigth = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRigth,
            orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }

}