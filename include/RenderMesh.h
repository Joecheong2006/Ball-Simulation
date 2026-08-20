#pragma once

#include "MeshData.h"
#include "gl/Buffer.h"

#include "BatchRenderer.h"

struct RenderMesh {
    gl::BufferLayout layout;
    gl::Buffer Vbo, Ebo;
    MeshData meshData;

    RenderMesh(MeshData &&meshData) noexcept;
    void initialize();
};

