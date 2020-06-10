#ifndef _LAYER_H
#define _LAYER_H

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

#include <string>

namespace Hazel {

    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event)  {}

        inline const std::string& GetName() const { return m_DebugName; }
    private:
        std::string m_DebugName;
    };

}

#endif  // _LAYER_H