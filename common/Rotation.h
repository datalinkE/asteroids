#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

glm::quat rotationBetweenVectorsQ(glm::vec3 start, glm::vec3 dest);

glm::mat4 rotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

