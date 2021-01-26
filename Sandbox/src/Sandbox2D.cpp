#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1200.0f / 900.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    HZ_PROFILE_FUNCTION();

    m_CheckboardTexture = Hazel::Texture2D::Create("Sandbox/assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    HZ_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);

    {
        HZ_PROFILE_SCOPE("Renderer Prep");

        // Render
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Hazel::RenderCommand::Clear();
    }

    {
        HZ_PROFILE_SCOPE("Renderer Draw");

        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Hazel::Renderer2D::DrawQuad({ -1.0f, 0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.2f, 0.6f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckboardTexture);

        Hazel::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
