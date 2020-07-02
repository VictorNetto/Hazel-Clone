#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer()
        : Layer("Example") {}
    
    void OnUpdate() override
    {
        // HZ_CLIENT_INFO("ExampleLayer::Update");
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
    }

    void OnEvent(Hazel::Event& event) override
    {
        HZ_CLIENT_TRACE("{0}", event);
    }
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Hazel::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};

Hazel::Application * Hazel::CreateApplication()
{
    return new Sandbox;
}