#ifndef _RENDER_COMMAND_H
#define _RENDER_COMMAND_H

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

    class RenderCommand {
    public:
        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static RendererAPI * s_RendererAPI;
    };

}

#endif // _RENDER_COMMAND_H