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

    Hazel::FramebufferSpecification fbSpec;
    fbSpec.Width = 1200;
    fbSpec.Height = 900;
    m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);
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

        m_Framebuffer->Bind();
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
        m_Framebuffer->Unbind();
    }
}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    static bool dockspaceOpen;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    
    auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    // textureID = 3;
    // uint32_t textureID = m_CheckboardTexture->GetRendererID();
    ImGui::Image((void*)textureID, ImVec2(120.0f, 90.0f));

    ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
