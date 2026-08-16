#include "gl/Buffers.h"

namespace gl {
    Buffers::Buffers(int target): target(target) {}

    void Buffers::initialize(int num) {
        ids.resize(num);
        glGenBuffers(num, ids.data());
    }

    void Buffers::bind(int index) const {
        glBindBuffer(target, ids[index]);
    }

    void Buffers::unbind() const {
        glBindBuffer(target, 0);
    }

    void Buffers::setData(size_t bytes, const void *data, int usage) const {
        glBufferData(target, bytes, data, usage);
    }

};
