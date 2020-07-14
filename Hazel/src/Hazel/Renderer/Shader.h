#ifndef _SHADER_H
#define _SHADER_H

#include <string>

namespace Hazel {

    class Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;
    private:
        uint32_t m_RendererID;
    };

}

#endif // _SHADER_H