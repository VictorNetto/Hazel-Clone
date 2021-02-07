#include <Hazel.h>

#include <array>

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
        OrthographicCameraController m_CameraController;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity, m_SecondaryCameraEntity;
        bool m_PrimaryCamera = true;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    };

}