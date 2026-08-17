#include "Transform2D.h"
#include "profiling.h"

glm::mat4 Transform2D::toMat4() {
    ZoneScoped;
    float c = std::cos(angle);
    float s = std::sin(angle);
    glm::mat4 m(1.0f);
    m[0] = glm::vec4(c * scale.x, s * scale.x, 0.0f, 0.0f);
    m[1] = glm::vec4(-s * scale.y, c * scale.y, 0.0f, 0.0f);
    m[3] = glm::vec4(position.x, position.y, 0.0f, 1.0f);
    return m;
}


void Transform2D::SoA::append(const SoA *transforms) {
    ZoneScoped;
    const int size = static_cast<int>(positions.size());
    const int diff = static_cast<int>(transforms->positions.size());

    positions.reserve(size + diff);
    scales.reserve(size + diff);
    angles.reserve(size + diff);

    positions.insert(positions.end(),
            transforms->positions.begin(), transforms->positions.end());

    scales.insert(scales.end(),
            transforms->scales.begin(), transforms->scales.end());

    angles.insert(angles.end(),
            transforms->angles.begin(), transforms->angles.end());
}

void Transform2D::SoA::append(const Container &transforms) {
    ZoneScoped;
    if (transforms.is<Transform2D::SoA>()) {
        this->append(transforms.cast<Transform2D::SoA>());
    }
    assert(true); // Unkown container type
}


void Transform2D::AoS::append(const AoS *transforms) {
    ZoneScoped;
    const int size = static_cast<int>(this->transforms.size());
    const int diff = static_cast<int>(transforms->transforms.size());

    this->transforms.reserve(size + diff);
    this->transforms.insert(this->transforms.end(),
            transforms->transforms.begin(), transforms->transforms.end());
}

void Transform2D::AoS::append(const Container &transforms) {
    ZoneScoped;
    if (transforms.is<Transform2D::AoS>()) {
        this->append(transforms.cast<Transform2D::AoS>());
    }
    assert(true);
}
