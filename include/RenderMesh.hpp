#pragma once

#include "MeshData.hpp"
#include "gl/Buffer.hpp"

#include "BatchRenderer.hpp"

class RenderMesh {
    gl::BufferLayout layout;
    gl::Buffer Vbo, Ebo;
    MeshData meshData;

public:
    RenderMesh(MeshData &&meshData) noexcept;
    void initialize();
    void bindRendererLayout(const BatchRenderer &renderer) const;
    void activate() const;

    inline int getIndexCount() const { return static_cast<int>(meshData.indices.size()); }
    inline int getVertexCount() const { return static_cast<int>(meshData.vertex.size()); }

};

