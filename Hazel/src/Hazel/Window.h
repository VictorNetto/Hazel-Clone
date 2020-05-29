#ifndef _WINDOW_H
#define _WINDOW_H

#include "Hazel/Events/Event.h"

#include <string>
#include <functional>

namespace Hazel {

    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Hazel Engine",
                        unsigned int width = 800,
                        unsigned int height = 600)
            : Title(title), Width(width), Height(height) {}
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void * GetNativeWindow() const = 0;

        static Window * Create(const WindowProps& props = WindowProps());
    };

}

#endif  // _WINDOW_H