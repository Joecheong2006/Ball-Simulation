#pragma once

#include <vector>
#include "gl/BufferLayout.hpp"

struct MeshData {
    std::vector<float> vertex;
    std::vector<uint32_t> indices;
    gl::BufferLayout::Attributes vertexAttributes;
};

