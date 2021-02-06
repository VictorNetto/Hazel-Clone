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
        entt::entity m_SquareEntity;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    };

}