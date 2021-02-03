#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Hazel/Core/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"

#include <memory>
#include <string>

namespace Hazel {

    class Application {
    public:
        Application(const std::string& name = "Hazel App");
        virtual ~Application();

        void PushLayer(Layer * layer);
        void PushOverlay(Layer * overlay);

        void OnEvent(Event& e);
        void Run();

        inline Window& GetWindow() { return *m_Window; }

        void Close();

        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer * m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H