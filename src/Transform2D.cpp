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

void Transform2D::SoA::append(const Container &transforms) {
    ZoneScoped;
    append(transforms, transforms.size());
}

void Transform2D::SoA::append(const Container &transforms, int size) {
    ZoneScoped;
    if (transforms.is<Transform2D::SoA>()) {
        this->append(*transforms.cast<Transform2D::SoA>(), size);
    }
    assert(true && "Unkown container type");
}

void Transform2D::SoA::append(const SoA &transforms) {
    append(transforms, static_cast<int>(transforms.size()));
}

void Transform2D::SoA::append(const SoA &transforms, int size) {
    ZoneScoped;
    positions.insert(positions.end(),
            transforms.positions.begin(), transforms.positions.begin() + size);

    scales.insert(scales.end(),
            transforms.scales.begin(), transforms.scales.begin() + size);

    angles.insert(angles.end(),
            transforms.angles.begin(), transforms.angles.begin() + size);
}

void Transform2D::AoS::append(const Container &transforms) {
    ZoneScoped;
    append(transforms, transforms.size());
}

void Transform2D::AoS::append(const Container &transforms, int size) {
    ZoneScoped;
    if (transforms.is<Transform2D::AoS>()) {
        this->append(*transforms.cast<Transform2D::AoS>(), size);
    }
    assert(true && "Unkown container type");
}

void Transform2D::AoS::append(const AoS &transforms) {
    ZoneScoped;
    append(transforms, static_cast<int>(transforms.size()));
}

void Transform2D::AoS::append(const AoS &transforms, int size) {
    ZoneScoped;
    this->transforms.insert(this->transforms.end(),
            transforms.transforms.begin(), transforms.transforms.begin() + size);
}

Transform2D::Container::Container() noexcept
    : TaggedPointer(new Transform2D::SoA)
{}

void Transform2D::Container::add(const Transform2D &transform) {
    Dispatch([&transform](auto *obj) {
            obj->add(transform);
        });
}

void Transform2D::Container::append(const Container &transforms) {
    Dispatch([&transforms](auto *obj) {
            obj->append(transforms);
        });
}

void Transform2D::Container::append(const Container &transforms, int size) {
    Dispatch([&transforms, &size](auto *obj) {
            obj->append(transforms, size);
        });
}

int Transform2D::Container::size() const {
    return Dispatch([&](auto *obj) {
            return obj->size();
        });
}

Transform2D Transform2D::Container::at(int i) const {
    return Dispatch([&](auto *obj) {
            return obj->at(i);
        });
}

glm::vec2 Transform2D::Container::getPositionAt(int i) const {
    return Dispatch([&](auto *obj) {
            return obj->getPositionAt(i);
        });
}

float Transform2D::Container::getAngleAt(int i) const {
    return Dispatch([&](auto *obj) {
            return obj->getAngleAt(i);
        });
}

void Transform2D::Container::setPositionAt(int i, const glm::vec2 &position) {
    Dispatch([&i, &position](auto *obj) {
            obj->setPositionAt(i, position);
        });
}

void Transform2D::Container::setAngleAt(int i, float angle) {
    Dispatch([&i, &angle](auto *obj) {
            obj->setAngleAt(i, angle);
        });
}

void Transform2D::Container::clear() {
    Dispatch([](auto *obj) {
            obj->clear();
        });
}
