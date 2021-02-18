#include "EditorLayer.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "Hazel/Scene/SceneSerializer.h"

#include <filesystem>
#include <vector>

namespace Hazel {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1200;
        fbSpec.Height = 900;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = std::make_shared<Scene>("Empty Scene");
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HZ_PROFILE_FUNCTION();

        // Resize
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y >= 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.205f, 0.21f, 1.0f });
        RenderCommand::Clear();

        // Update Scene
        m_ActiveScene->OnUpdate(ts);

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
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
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 300.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("New...", "Ctrl+N"))
                    NewScene();
                
                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();
                
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveScene();
                
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (m_NewScene)
            ImGui::OpenPopup("New Scene");

        if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char buffer[256] = "Scene Name";
            ImGui::InputText("##Scene Name", buffer, sizeof(buffer));

            if (ImGui::Button("Create", ImVec2(120, 0)))
            {
                m_ActiveScene = std::make_shared<Scene>(buffer);
			    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

                m_NewScene = false;
                strcpy(buffer, "Scene Name");
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                m_NewScene = false;
                strcpy(buffer, "Scene Name");
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (m_OpenScene)
            ImGui::OpenPopup("Open Scene");

        if (ImGui::BeginPopupModal("Open Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            std::vector<std::string> scenePaths;
            std::vector<std::string> sceneNames;
            std::vector<uint64_t> sceneSizes;
            
            for (auto& p : std::filesystem::directory_iterator("Hazel-Editor/scenes/"))
            {
                if (p.path().extension() == ".hazel")
                {
                    scenePaths.push_back(p.path());
                    sceneNames.push_back(std::string(p.path()).substr(20));
                    sceneSizes.push_back(std::filesystem::file_size(p.path()));
                }
            }

            static int itemSelected = -1;
            bool pressed;

            for (int i = 0; i < sceneNames.size(); i++)
            {
                pressed = ImGui::Selectable(sceneNames[i].c_str(), itemSelected == i, ImGuiSelectableFlags_DontClosePopups);
                ImGui::SameLine();
                ImGui::Text("\t\t%10lu kB", sceneSizes[i] / 1024);
                if (pressed) itemSelected = i;
            }

            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                m_ActiveScene = std::make_shared<Scene>(sceneNames[itemSelected]);
			    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

                SceneSerializer serializer(m_ActiveScene);
                serializer.Deserialize(scenePaths[itemSelected]);

                m_OpenScene = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                m_OpenScene = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }  // Open Scene

        m_SceneHierarchyPanel.OnImGuiRenderer();

        ImGui::Begin("Stats");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
        {
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)(uint64_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    void EditorLayer::NewScene()
    {
        m_NewScene = true;
    }

    void EditorLayer::OpenScene()
    {
        m_OpenScene = true;
    }

    void EditorLayer::SaveScene()
    {
        SceneSerializer serializer(m_ActiveScene);
        std::string filepath = "Hazel-Editor/scenes/" + m_ActiveScene->GetName() + ".hazel";

        serializer.Serialize(filepath);
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
            case Key::N:
            {
                if (control)
                    NewScene();

                break;
            }
            case Key::O:
            {
                if (control)
                    OpenScene();

                break;
            }
            case Key::S:
            {
                if (control)
                    SaveScene();

                break;
            }

            // // Gizmos
            // case Key::Q:
            // {
            // 	if (!ImGuizmo::IsUsing())
            // 		m_GizmoType = -1;
            // 	break;
            // }
            // case Key::W:
            // {
            // 	if (!ImGuizmo::IsUsing())
            // 		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            // 	break;
            // }
            // case Key::E:
            // {
            // 	if (!ImGuizmo::IsUsing())
            // 		m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            // 	break;
            // }
            // case Key::R:
            // {
            // 	if (!ImGuizmo::IsUsing())
            // 		m_GizmoType = ImGuizmo::OPERATION::SCALE;
            // 	break;
            // }
        }

        return true;
        
    }

}