#include "RenderMesh.h"

#include "profiling.h"

RenderMesh::RenderMesh(MeshData &&meshData) noexcept
    : Vbo(GL_ARRAY_BUFFER), Ebo(GL_ELEMENT_ARRAY_BUFFER), meshData(std::move(meshData))
{
    ZoneScoped;
}

void RenderMesh::initialize() {
    auto &vertexAttributes = meshData.vertexAttributes;
    auto &vertex = meshData.vertex;
    auto &indices = meshData.indices;

    layout.initialize();
    layout.bind();

    // Bind EBO so that VAO can related it
    if (!meshData.indices.empty()) {
        Ebo.initialize();
        Ebo.bind();
        Ebo.setData(indices.size() * sizeof(uint32_t), indices.data());
    }

    // Bind VBO so that we can set vertex attribute
    Vbo.initialize();
    Vbo.bind();
    Vbo.setData(vertex.size() * sizeof(float), vertex.data());
    layout.set(vertexAttributes);
}

void RenderMesh::bindRendererLayout(const BatchRenderer &renderer) const {
    renderer.bindLayout(layout);
}

void RenderMesh::activate() const {
    layout.bind();
}
