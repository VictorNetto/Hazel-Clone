#ifndef _RENDERER_H
#define _RENDERER_H

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/OrthographicCamera.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hazel {

    class Renderer {
    public:
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData * s_SceneData;
    };

}

#endif // _RENDERER_H