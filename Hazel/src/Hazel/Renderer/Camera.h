#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>

namespace Hazel {

    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_Projection(projection) {}

        ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_Projection; }

    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };

}

#endif // _CAMERA_H