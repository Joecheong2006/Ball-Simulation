#pragma once

#include <cstddef>
#include <glad/gl.h> 
#include <vector>

namespace gl {
    class Buffers {
    private:
        std::vector<unsigned int> ids;
        int target{};

    public:
        explicit Buffers(int target);
        ~Buffers();
        void initialize(int num);

        void bind(int index) const;
        void unbind() const;
        void setData(size_t bytes, const void *data, int usage = GL_STATIC_DRAW) const;
    };

};
