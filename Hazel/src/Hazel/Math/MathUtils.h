#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include <glm/glm.hpp>

namespace Hazel::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

}

#endif // _MATH_UTILS_H