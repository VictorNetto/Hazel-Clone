#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <memory>
#include "Hazel/Events/ApplicationEvent.h"
#include "Window.h"

namespace Hazel {

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H