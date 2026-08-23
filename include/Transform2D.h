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
        void append(const Container &transforms, int size);
        void append(const SoA &transforms);
        void append(const SoA &transforms, int size);

        inline void add(const Transform2D &transform) {
            positions.push_back(transform.position);
            scales.push_back(transform.scale);
            angles.push_back(transform.angle);
        }

        inline int size() const {
            return static_cast<int>(positions.size());
        }

        inline Transform2D at(int i) const {
            return { positions[i], scales[i], angles[i] };
        }

        inline glm::vec2 getPositionAt(int i) const {
            return positions[i];
        }

        inline float getAngleAt(int i) const {
            return angles[i];
        }

        inline void setPositionAt(int i, const glm::vec2 &position) {
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
        void append(const Container &transforms, int size);
        void append(const AoS &transforms);
        void append(const AoS &transforms, int size);

        inline void add(const Transform2D &transform) {
            transforms.push_back(transform);
        }

        inline int size() const {
            return static_cast<int>(transforms.size());
        }

        inline Transform2D at(int i) const {
            return transforms[i];
        }

        inline glm::vec2 getPositionAt(int i) const {
            return transforms[i].position;
        }

        inline float getAngleAt(int i) const {
            return transforms[i].angle;
        }

        inline void setPositionAt(int i, const glm::vec2 &position) {
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

        Container() noexcept;

        void add(const Transform2D &transform);
        void append(const Container &transforms);
        void append(const Container &transforms, int size);

        int size() const;
        Transform2D at(int i) const;
        glm::vec2 getPositionAt(int i) const;
        float getAngleAt(int i) const;
        void setPositionAt(int i, const glm::vec2 &position);
        void setAngleAt(int i, float angle);
        void clear();
    };

};

