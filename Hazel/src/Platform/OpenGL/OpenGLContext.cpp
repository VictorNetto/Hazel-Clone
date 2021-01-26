#include "Platform/OpenGL/OpenGLContext.h"

#include "Hazel/Renderer/RendererInfo.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"

#include "Hazel/Debug/Instrumentor.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel {

    OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
        : m_WindowHandle(windowHandle)
    {
        HZ_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::Init()
    {
        HZ_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        RendererInfo::SetInfo(
            "OpenGL",
            (const char *) glGetString(GL_VENDOR),
            (const char *) glGetString(GL_RENDERER),
            (const char *) glGetString(GL_VERSION)
        );
    }

    void OpenGLContext::SwapBuffers()
    {
        HZ_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }


}