#ifndef _INPUT_H
#define _INPUT_H

#include <utility>

namespace Hazel {

    class Input {
    public:
        static bool IsKeyPressed(int keycode);

        static bool IsMouseButtonPressed(int button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}

#endif // _INPUT_H