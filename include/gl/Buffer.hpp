#pragma once

#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    class Buffer {
    private:
        unsigned int id{};
        int target;

    public:
        explicit Buffer(int target);
        ~Buffer();
        void initialize();

        void bind() const;
        void unbind() const;
        void setData(size_t bytes, const void *data, int usage = GL_STATIC_DRAW) const;
    };

};
