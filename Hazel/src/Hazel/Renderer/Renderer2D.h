#ifndef _RENDERER_2D_H
#define _RENDERER_2D_H

#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    };

}

#endif // _RENDERER_2D_H