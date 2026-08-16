#pragma once

#include "MeshData.h"
#include "gl/Buffer.h"

struct RenderMesh {
    gl::BufferLayout layout;
    gl::Buffer Vbo, Ebo;

    RenderMesh(MeshData &meshData);
};

