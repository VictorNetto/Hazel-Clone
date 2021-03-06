#ifndef _LAYER_STACK_H
#define _LAYER_STACK_H

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Layer.h"

#include <vector>

namespace Hazel {

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer * layer);
        void PushOverlay(Layer * overlay);
        void PopLayer(Layer * layer);
        void PopOverlay(Layer * overlay);

        std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer *>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer *> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };

}

#endif  // _LAYER_STACK_H