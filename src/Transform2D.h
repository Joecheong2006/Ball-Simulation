#pragma once

#include <glm/glm.hpp>
#include "TaggedPointer.h"

struct Transform2D {
    glm::vec2 position, scale;
    float angle;

    glm::mat4 toMat4();

    struct Container;

    struct SoA {
        std::vector<glm::vec2> positions, scales;
        std::vector<float> angles;

        void append(const Container &transforms);
        void append(const SoA *transforms);

        inline void add(const Transform2D &transform) {
            positions.push_back(transform.position);
            scales.push_back(transform.scale);
            angles.push_back(transform.angle);
        }

        inline glm::vec2 getPositionAt(int i) const {
            return positions[i];
        }

        inline float getAngleAt(int i) const {
            return angles[i];
        }

        inline void setPositionAt(int i, glm::vec2 position) {
            positions[i] = position;
        }

        inline void setAngleAt(int i, float angle) {
            angles[i] = angle;
        }

        inline void clear() {
            positions.clear();
            scales.clear();
            angles.clear();
        }
    };

    struct AoS {
        std::vector<Transform2D> transforms;

        void append(const Container &transforms);
        void append(const AoS *transforms);

        inline void add(const Transform2D &transform) {
            transforms.push_back(transform);
        }

        inline glm::vec2 getPositionAt(int i) const {
            return transforms[i].position;
        }

        inline float getAngleAt(int i) const {
            return transforms[i].angle;
        }

        inline void setPositionAt(int i, glm::vec2 position) {
            transforms[i].position = position;
        }

        inline void setAngleAt(int i, float angle) {
            transforms[i].angle = angle;
        }

        inline void clear() {
            transforms.clear();
        }
    };

    struct Container : public TaggedPointer<SoA, AoS> {
        using TaggedPointer::TaggedPointer;
        void add(const Transform2D &transform) {
            Dispatch([&transform](auto *obj) {
                    obj->add(transform);
                });
        }

        glm::vec2 getPositionAt(int i) const {
            return Dispatch([&](auto *obj) {
                    return obj->getPositionAt(i);
                });
        }

        float getAngleAt(int i) const {
            return Dispatch([&](auto *obj) {
                    return obj->getAngleAt(i);
                });
        }

        void setPositionAt(int i, glm::vec2 position) {
            Dispatch([&i, &position](auto *obj) {
                    obj->setPositionAt(i, position);
                });
        }

        void setAngleAt(int i, float angle) {
            Dispatch([&i, &angle](auto *obj) {
                    obj->setAngleAt(i, angle);
                });
        }

        void append(const Container &transforms) {
            Dispatch([&transforms](auto *obj) {
                    obj->append(transforms);
                });
        }

        void clear() {
            Dispatch([](auto *obj) {
                    obj->clear();
                });
        }

    };

};

