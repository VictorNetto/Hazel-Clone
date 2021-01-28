#include <Hazel.h>

#include <array>

class Sandbox2D : public Hazel::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;
private:
    Hazel::OrthographicCameraController m_CameraController;
    Hazel::Ref<Hazel::Texture2D> m_CheckboardTexture, m_ChernoTexture;

    struct QuadProp
    {
        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 color;
    };
    const int m_nQuads = 200;
    std::array<QuadProp, 200> m_QuadsProps;
};