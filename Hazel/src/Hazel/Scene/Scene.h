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
        Scene(const std::string& name);
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        const std::string& GetName() const { return m_Name; }

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        std::string m_Name;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}

#endif // _SCENE_H