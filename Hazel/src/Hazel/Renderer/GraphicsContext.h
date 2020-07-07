#ifndef _GRAPHICS_CONTEXT_H
#define _GRAPHICS_CONTEXT_H

namespace Hazel {

    class GraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}

#endif // _GRAPHICS_CONTEXT_H