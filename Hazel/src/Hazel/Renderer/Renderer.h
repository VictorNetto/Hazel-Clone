#ifndef _RENDERER_H
#define _RENDERER_H

namespace Hazel {

    enum class RendererAPI {
        None = 0, OpenGL = 1
    };

    class Renderer {
    public:
        inline static RendererAPI GetAPI() { return s_RendererAPI; }
    private:
        static RendererAPI s_RendererAPI;
    };

}

#endif // _RENDERER_H