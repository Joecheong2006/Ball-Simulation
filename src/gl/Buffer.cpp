#include "gl/Buffer.h"
#include <assert.h>

namespace gl {
    Buffer::Buffer(int target): target(target) {}

    void Buffer::initialize() {
        glGenBuffers(1, &id);
    }

    void Buffer::bind() const {
        glBindBuffer(target, id);
    }

    void Buffer::unbind() const {
        glBindBuffer(target, 0);
    }

    void Buffer::setData(size_t bytes, const void *data, int usage) const {
        glBufferData(target, bytes, data, usage);
    }

};
