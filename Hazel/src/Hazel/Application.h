#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"
#include "Hazel/LayerStack.h"

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
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H