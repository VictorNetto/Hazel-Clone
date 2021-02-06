#ifndef _SCENE_H
#define _SCENE_H

#include <entt.hpp>

#include "Hazel/Core/Timestep.h"

namespace Hazel {

    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // TEMP
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep ts);    
    private:
        entt::registry m_Registry;
    };

}

#endif // _SCENE_H