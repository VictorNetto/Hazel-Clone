#ifndef _OPENGL_CONTEXT_H
#define _OPENGL_CONTEXT_H

#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel {

    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow * windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow * m_WindowHandle;
    };

}

#endif // _OPENGL_CONTEXT_H