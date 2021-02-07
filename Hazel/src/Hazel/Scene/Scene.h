#ifndef _SCENE_H
#define _SCENE_H

#include <string>
#include <entt.hpp>

#include "Hazel/Core/Timestep.h"

namespace Hazel {

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        void OnUpdate(Timestep ts);    
    private:
        entt::registry m_Registry;

        friend Entity;
    };

}

#endif // _SCENE_H