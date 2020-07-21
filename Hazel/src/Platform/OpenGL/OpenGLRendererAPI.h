#ifndef _OPENGL_RENDERER_API_H
#define _OPENGL_RENDERER_API_H

#include "Hazel/Renderer/RendererAPI.h"

struct GLFWwindow;

namespace Hazel {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void Init() override;
        
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };

}

#endif // _OPENGL_RENDERER_API_H