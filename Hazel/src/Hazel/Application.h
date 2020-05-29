#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <memory>
#include "Window.h"

namespace Hazel {

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H