#ifndef _SCRIPTABLE_ENTITY_h
#define _SCRIPTABLE_ENTITY_h

#include "Hazel/Scene/Entity.h"

namespace Hazel {

    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {};

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
    
    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Timestep ts) {}
        
    private:
        Entity m_Entity;
        friend class Scene;
    };

}

#endif // _SCRIPTABLE_ENTITY_h