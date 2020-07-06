#ifndef _LINUX_INPUT_H
#define _LINUX_INPUT_H

#include "Hazel/Input.h"

namespace Hazel {

    class LinuxInput : public Input {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;

        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };

}

#endif // _LINUX_INPUT_H