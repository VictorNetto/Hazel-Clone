#ifndef _SHADER_H
#define _SHADER_H

#include <string>

namespace Hazel {

    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader * Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };

}

#endif // _SHADER_H