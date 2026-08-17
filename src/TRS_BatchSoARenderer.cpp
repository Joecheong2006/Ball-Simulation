#include "TRS_BatchSoARenderer.h"
#include "profiling.h"

void TRS_BatchSoARenderer::initialize() {
    ZoneScoped;
    buffers.initialize(3);
}

void TRS_BatchSoARenderer::bindRenderMesh(RenderMesh &renderMesh) {
    ZoneScoped;
    renderMesh.layout.bind();

    buffers.bind(0);
    renderMesh.layout.set({ GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(1);
    renderMesh.layout.set({ GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(2);
    renderMesh.layout.set({ GL_FLOAT, 0, 1, 1, 1 });
}

void TRS_BatchSoARenderer::submit(const Transform2D &transform) {
    ZoneScoped;
    batch.add(transform);
}

void TRS_BatchSoARenderer::submitBatch(Transform2D::Container &transforms) {
    ZoneScoped;
    batch.append(transforms);
}

void TRS_BatchSoARenderer::render(RenderMesh &renderMesh, RenderMaterial &renderMat) {
    ZoneScoped;
    buffers.bind(0);
    buffers.setData(batch.positions.size() * sizeof(glm::vec2), batch.positions.data());
    buffers.bind(1);
    buffers.setData(batch.scales.size() * sizeof(glm::vec2), batch.scales.data());
    buffers.bind(2);
    buffers.setData(batch.angles.size() * sizeof(float), batch.angles.data());

    renderMat.shaderProgram.use();
    renderMesh.layout.bind();

    renderMat.shaderProgram.setUniformMat4("projection", cachedCameraProjection);
    renderMat.shaderProgram.setUniform2f("camPos", cachedCameraPosition);

    int size = static_cast<int>(batch.positions.size());
    glDrawElementsInstanced(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)0, size);

    batch.clear();
}
