#ifndef _IMGUI_LAYER_H
#define _IMGUI_LAYER_H

#include "Hazel/Layer.h"

namespace Hazel {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& even);
    private:
        float m_Time;
    };

}

#endif // _IMGUI_LAYER_H