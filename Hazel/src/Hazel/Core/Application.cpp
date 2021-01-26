#include "Hazel/Core/Application.h"

#include "Hazel/Core/Log.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Debug/Instrumentor.h"

#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_PROFILE_FUNCTION();

        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        HZ_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer * layer)
    {
        HZ_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer * layer)
    {
        HZ_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();

    }

    void Application::OnEvent(Event& e)
    {
        HZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        HZ_PROFILE_FUNCTION();

        while (m_Running) {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            
            RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
            RenderCommand::Clear();

            if (!m_Minimized)
            {
                for (Layer * layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }
            
            m_ImGuiLayer->Begin();
            for (Layer * layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();
        
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

}