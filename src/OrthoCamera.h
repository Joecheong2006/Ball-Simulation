#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct OrthoCamera {
    glm::mat4 projection;
    glm::vec2 position;

    OrthoCamera(glm::vec2 position, glm::vec2 viewportSize, float near, float far);

};

