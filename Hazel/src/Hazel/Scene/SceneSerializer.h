#ifndef _SCENE_SERIALIZER_H
#define _SCENE_SERIALIZER_H

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"

namespace Hazel {

    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);

    private:
        Ref<Scene> m_Scene;
    };

}

#endif  // _SCENE_SERIALIZER_H