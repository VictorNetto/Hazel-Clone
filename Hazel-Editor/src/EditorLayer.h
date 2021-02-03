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
        Ref<Texture2D> m_CheckboardTexture, m_ChernoTexture;
        Ref<Framebuffer> m_Framebuffer;

        struct QuadProp
        {
            glm::vec3 position;
            glm::vec2 size;
            glm::vec4 color;
            float rotationSpeed;
        };
        const int m_nQuads = 200;
        std::array<QuadProp, 200> m_QuadsProps;
    };

}