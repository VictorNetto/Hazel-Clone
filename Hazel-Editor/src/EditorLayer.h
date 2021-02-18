#include <Hazel.h>

#include <array>

#include "Panels/SceneHierarchyPanel.h"

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
        bool OnKeyPressed(KeyPressedEvent& e) { return false; }

        void NewScene();
        void OpenScene();
        void SaveSceneAs();
    private:
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };

}