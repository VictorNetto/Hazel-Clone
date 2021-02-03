#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include <time.h>
#include <stdlib.h>

static float RandomFloat(float a, float b)
{
    // n is a random float number between 0.0f and 1.0f
    float n = (float)rand() / (float)RAND_MAX;

    return a + (b - a) * n;
}

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1200.0f / 900.0f, true)
{
    srand(time(NULL));

    for (int i = 0; i < m_nQuads; i++)
    {
        m_QuadsProps[i].position = { RandomFloat(-3.0f, 3.0f), RandomFloat(-3.0f, 3.0f), RandomFloat(-0.1, 0.1) };
        m_QuadsProps[i].size = { RandomFloat(-1.7f, 1.7f), RandomFloat(-1.7f, 1.7f) };
        m_QuadsProps[i].color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 1.0f };
        m_QuadsProps[i].rotationSpeed = RandomFloat(-300.0f, 300.0f);
    }
}

void Sandbox2D::OnAttach()
{
    HZ_PROFILE_FUNCTION();

    m_CheckboardTexture = Hazel::Texture2D::Create("Sandbox/assets/textures/Checkerboard.png");
    m_ChernoTexture = Hazel::Texture2D::Create("Sandbox/assets/textures/ChernoLogo.png");
}

void Sandbox2D::OnDetach()
{
    HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    HZ_PROFILE_FUNCTION();

    static float time = 0;
    time += ts;

    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::Renderer2D::ResetStats();
    {
        HZ_PROFILE_SCOPE("Renderer Prep");

        Hazel::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.9f, 1.0f });
        Hazel::RenderCommand::Clear();
    }

    {
        HZ_PROFILE_SCOPE("Renderer Draw");

        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (int i = 0; i < m_nQuads; i++)
        {
            glm::vec3 position = m_QuadsProps[i].position;
            glm::vec2 size = m_QuadsProps[i].size;
            glm::vec4 color = m_QuadsProps[i].color;
            float rotation = m_QuadsProps[i].rotationSpeed * time;
            Hazel::Renderer2D::DrawRotatedQuad(position, rotation, size, color);
        }
        // Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f }, glm::radians(30.0f), { 1.2f, 0.6f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckboardTexture, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
        Hazel::Renderer2D::DrawRotatedQuad({ -5.0f, 5.0f, 0.1f }, time * 45.0f, { 1.0f, 1.0f }, m_ChernoTexture, 2);

        Hazel::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    
    auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
