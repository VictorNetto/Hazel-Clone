#include "EditorLayer.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <ImGuizmo.h>

#include "Hazel/Math/MathUtils.h"
#include "Hazel/Scene/SceneSerializer.h"

#include <filesystem>
#include <vector>
#include <algorithm>

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
        fbSpec.Atttachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = std::make_shared<Scene>("Empty Scene");
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_EditorCamera = EditorCamera(30.0f, 1.333f, 0.1f, 1000.0f);
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
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        m_EditorCamera.OnUpdate(ts);

        // Render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.205f, 0.21f, 1.0f });
        RenderCommand::Clear();

        // Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

        // Update Scene
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        auto[mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            // m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

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
        {
            ImGui::SetNextWindowSize(ImVec2{ 300, 120 });
            ImGui::OpenPopup("New Scene");
        }

        if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char buffer[256] = { 0 };
            ImGui::SetCursorPos(ImVec2{ 10, 35 });
            ImGui::PushItemWidth(280);
            ImGui::InputTextWithHint("##Scene Name", "Scene Name", buffer, sizeof(buffer));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2{ 0, 70 });
            ImGui::Separator();

            ImGui::SetCursorPos(ImVec2{ 10, 85 });
            if (ImGui::Button("Create", ImVec2(120, 0)))
            {
                m_ActiveScene = std::make_shared<Scene>(buffer);
			    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

                m_NewScene = false;
                memset(buffer, 0, sizeof(buffer));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            
            ImGui::SetCursorPos(ImVec2{ 170, 85 });
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                m_NewScene = false;
                memset(buffer, 0, sizeof(buffer));
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (m_OpenScene)
        {
            ImGui::SetNextWindowSize(ImVec2{ 400, 400 });
            ImGui::OpenPopup("Open Scene");
        }

        if (ImGui::BeginPopupModal("Open Scene", NULL, ImGuiWindowFlags_NoResize))
        {
            static int itemSelected = -1;
            bool pressed;

            for (int i = 0; i < m_SceneFileNames.size(); i++)
            {
                pressed = ImGui::Selectable(m_SceneFileNames[i].c_str(), itemSelected == i, ImGuiSelectableFlags_DontClosePopups);
                if (pressed) itemSelected = i;
            }

            // Right-click on a blank space
            if (ImGui::BeginPopupContextWindow(0, 1, false))
            {
                if (ImGui::MenuItem("Update Scene Files"))
                {
                    UpdateSceneFilesInfos();
                }

                ImGui::EndPopup();
            }


            ImGui::SetCursorPos(ImVec2{ 0, 350 });
            ImGui::Separator();

            ImGui::SetCursorPos(ImVec2{ 10, 365 });
            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                std::string fileName = m_SceneFileNames[itemSelected];
                m_ActiveScene = std::make_shared<Scene>(fileName);
			    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

                SceneSerializer serializer(m_ActiveScene);
                serializer.Deserialize("Hazel-Editor/scenes/" + fileName);

                m_OpenScene = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();

            ImGui::SetCursorPos(ImVec2{ 270, 365 });
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                m_OpenScene = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }  // Open Scene

        m_SceneHierarchyPanel.OnImGuiRenderer();

        ImGui::Begin("Stats");

        ImGui::Text("Renderer Informations:");
        ImGui::Text("Renderer API: %s", RendererInfo::GetRendererAPI().c_str());
        ImGui::Text("Vendor: %s", RendererInfo::GetVendor().c_str());
        ImGui::Text("Renderer: %s", RendererInfo::GetRenderer().c_str());
        ImGui::Text("Version: %s", RendererInfo::GetVersion().c_str());

        ImGui::Separator();

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        std::string viewportLabel = m_ActiveScene->GetName() + "###Viewport";
        ImGui::Begin(viewportLabel.c_str());

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        // m_ViewportFocused = ImGui::IsWindowFocused();
        // m_ViewportHovered = ImGui::IsWindowHovered();
        // Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
        {
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
        ImGui::Image((void*)(uint64_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        // ImGuizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Camera

            // Runtime camera from entity
            // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
            // const auto& camera = cameraEntity.GetComponent<CameraComponent>();
            // const glm::mat4& camearProjection = camera.Camera.GetProjection();
            // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            // Editor camera
            glm::mat4 camearProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            // Entity
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f;  // Snap to 0.5m for translation/scale
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;  // Snap to 45 degrees for rotation
            
            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(camearProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);
            
            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation,rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::ShowDemoWindow();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    void EditorLayer::NewScene()
    {
        m_NewScene = true;
    }

    void EditorLayer::OpenScene()
    {
        UpdateSceneFilesInfos();
        m_OpenScene = true;
    }

    void EditorLayer::SaveScene()
    {
        SceneSerializer serializer(m_ActiveScene);
        std::string filepath = "Hazel-Editor/scenes/" + m_ActiveScene->GetName() + ".hazel";

        serializer.Serialize(filepath);
    }

    void EditorLayer::UpdateSceneFilesInfos()
    {
        m_SceneFileNames.clear();
        
        for (auto& p : std::filesystem::directory_iterator("Hazel-Editor/scenes/"))
        {
            if (p.path().extension() == ".hazel")
            {
                m_SceneFileNames.push_back(std::string(p.path()).substr(20));
            }
        }
        
        std::sort(m_SceneFileNames.begin(), m_SceneFileNames.end());
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

            // Gizmos
            case Key::Q:
            {
            	if (!ImGuizmo::IsUsing())
            		m_GizmoType = -1;
            	break;
            }
            case Key::W:
            {
            	if (!ImGuizmo::IsUsing())
            		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            	break;
            }
            case Key::E:
            {
            	if (!ImGuizmo::IsUsing())
            		m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            	break;
            }
            case Key::R:
            {
            	if (!ImGuizmo::IsUsing())
            		m_GizmoType = ImGuizmo::OPERATION::SCALE;
            	break;
            }
        }

        return true;
        
    }

}