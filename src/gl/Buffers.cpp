#include "gl/Buffers.hpp"
#include "profiling.hpp"

namespace gl {
    Buffers::Buffers(int target): target(target) {}

    Buffers::~Buffers() {
        if (!ids.empty())
            glDeleteBuffers(static_cast<int>(ids.size()), ids.data());
    };

    void Buffers::initialize(int num) {
        ZoneScoped;
        ids.resize(num);
        glGenBuffers(num, ids.data());
    }

    void Buffers::bind(int index) const {
        ZoneScoped;
        glBindBuffer(target, ids[index]);
    }

    void Buffers::unbind() const {
        ZoneScoped;
        glBindBuffer(target, 0);
    }

    void Buffers::setData(size_t bytes, const void *data, int usage) const {
        ZoneScoped;
        glBufferData(target, bytes, data, usage);
    }

};
