#ifndef _APPLICATION_H
#define _APPLICATION_H

namespace Hazel {

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in client
    Application * CreateApplication();

}

#endif // _APPLICATION_H