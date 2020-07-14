#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Buffer.h"

#include <memory>

namespace Hazel {

    class Application {
    public:
        Application();
        virtual ~Application();

        void PushLayer(Layer * layer);
        void PushOverlay(Layer * overlay);

        void OnEvent(Event& e);
        void Run();

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer * m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader> m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;
    private:
        static Application* s_Instance;
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H