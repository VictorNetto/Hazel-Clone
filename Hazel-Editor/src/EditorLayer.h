#include <Hazel.h>

#include <array>

#include "Panels/SceneHierarchyPanel.h"
#include "Hazel/Renderer/EditorCamera.h"

namespace Hazel {
    
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& e);
    private:
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        EditorCamera m_EditorCamera;

        // bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

        int m_GizmoType = -1;

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    
    // Files related methods/members - Save scene, Open scene, ...
    private:
        void UpdateSceneFilesInfos();

        void NewScene();
        void OpenScene();
        void SaveScene();

    private:
        std::vector<std::string> m_SceneFileNames;

        bool m_NewScene = false;
        bool m_OpenScene = false;
    };

}