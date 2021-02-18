#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/Components.h"

namespace Hazel {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRenderer()
    {
        ImGui::Begin("Scene Hierarchy");
        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity { entityID, m_Context.get() };
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        // Right-click on a blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                m_Context->CreateEntity("Empty Entity");
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, "%s", tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Item"))
            {
                entityDeleted = true;
            }

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 85.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template<typename T, typename UIFuntion>
    static void DrawComponent(const char* name, Entity entity, UIFuntion uiFunction, bool removable = true)
    {
        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();

            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_FramePadding;

            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name);
            ImGui::PopStyleVar();
            
            bool removeComponent = false;
            if (removable)
            {
                float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5);
                if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
                {
                    ImGui::OpenPopup("ComponentSettings");
                }
                
                
                if (ImGui::BeginPopup("ComponentSettings"))
                {
                    if (ImGui::MenuItem("Remove Component"))
                        removeComponent = true;
                    ImGui::EndPopup();
                }
            }

            if (open)
            {
                uiFunction(component);

                ImGui::TreePop();
            }

            if (removable && removeComponent)
                entity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            char buffer[256];

            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");
        
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                if (!m_SelectionContext.HasComponent<CameraComponent>())
                    m_SelectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer"))
            {
                if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
        {
            DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        }, false);

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
        {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float perspectiveVerticaFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticaFov, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetOrthographicSize(glm::radians(perspectiveVerticaFov));
                
                float perspectiveNear = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near Clip", &perspectiveNear, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetPerspectiveNearClip(perspectiveNear);

                float perspectiveFar = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far Clip", &perspectiveFar, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetPerspectiveFarClip(perspectiveFar);
            }
            else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

                float orthoSize = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetOrthographicSize(orthoSize);
                
                float orthoNear = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetOrthographicNearClip(orthoNear);

                float orthoFar = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f, 0.0f, 0.0f, "%.1f"))
                    camera.SetOrthographicFarClip(orthoFar);
            }
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        });
    }

}