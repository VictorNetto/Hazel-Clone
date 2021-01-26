#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Renderer/Renderer2D.h"

#include "Hazel/Debug/Instrumentor.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

    Renderer::SceneData * Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        HZ_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        HZ_PROFILE_FUNCTION();

        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();

        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
        HZ_PROFILE_FUNCTION();
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        HZ_PROFILE_FUNCTION();
        
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

}