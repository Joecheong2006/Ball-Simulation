#include "gl/Buffer.h"
#include "profiling.h"

#include <assert.h>

namespace gl {
    Buffer::Buffer(int target): target(target) {}

    void Buffer::initialize() {
        ZoneScoped;
        glGenBuffers(1, &id);
    }

    void Buffer::bind() const {
        ZoneScoped;
        glBindBuffer(target, id);
    }

    void Buffer::unbind() const {
        ZoneScoped;
        glBindBuffer(target, 0);
    }

    void Buffer::setData(size_t bytes, const void *data, int usage) const {
        ZoneScoped;
        glBufferData(target, bytes, data, usage);
    }

};
