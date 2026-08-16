#pragma once

#include <cstddef>
#include <glad/gl.h> 
#include <vector>

namespace gl {
    struct Buffers {
        std::vector<unsigned int> ids;
        int target;

        explicit Buffers(int target);
        void initialize(int num);

        void bind(int index) const;
        void unbind() const;
        void setData(size_t bytes, const void *data, int usage = GL_STATIC_DRAW) const;
    };

};
