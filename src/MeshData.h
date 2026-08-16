#pragma once

#include <vector>
#include "gl/BufferLayout.h"

struct MeshData {
    std::vector<float> vertex;
    gl::BufferLayout::Attributes vertexAttributes;

    std::vector<uint32_t> indices;
};

