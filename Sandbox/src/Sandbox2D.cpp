#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1200.0f / 900.0f, true)
{
}

void Sandbox2D::OnAttach()
{}

void Sandbox2D::OnDetach()
{}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Hazel::RenderCommand::Clear();

    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Hazel::Renderer2D::DrawQuad({ -1.0f, 0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.2f, 0.6f }, { 0.2f, 0.8f, 0.3f, 1.0f });


    Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
