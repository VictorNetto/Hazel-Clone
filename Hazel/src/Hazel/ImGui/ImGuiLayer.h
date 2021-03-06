#ifndef _IMGUI_LAYER_H
#define _IMGUI_LAYER_H

#include "Hazel/Core/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

        void Begin();
        void End();

        void SetDarkThemedColor();
    private:
        float m_Time = 0.0f;
    };

}

#endif // _IMGUI_LAYER_H