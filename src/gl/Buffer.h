#pragma once

#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    struct Buffer {
        unsigned int id;
        int target;

        Buffer(int target);

        void bind() const;
        void unbind() const;
        void setData(size_t bytes, const void *data, int usage = GL_STATIC_DRAW) const;
    };

};
