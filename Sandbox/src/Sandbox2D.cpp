#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1200.0f / 900.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    m_SquareVA = Hazel::VertexArray::Create();

    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVB->SetLayout({
        { Hazel::ShaderDataType::Float3, "a_Position" },
    });

    uint32_t squareIndices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    Hazel::Ref<Hazel::IndexBuffer> squareIB;
    squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, 6));

    m_SquareVA->AddVertexBuffer(squareVB);
    m_SquareVA->SetIndexBuffer(squareIB);

    m_FlatColorShader = Hazel::Shader::Create("Sandbox/assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
