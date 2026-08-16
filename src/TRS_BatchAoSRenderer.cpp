#include "TRS_BatchAoSRenderer.h"

void TRS_BatchAoSRenderer::initialize() {
    buffer.initialize();
}

void TRS_BatchAoSRenderer::bindRenderMesh(RenderMesh &renderMesh) {
    renderMesh.layout.bind();

    buffer.bind();
    renderMesh.layout.set(gl::BufferLayout::Aggregate<Transform2D, float>(1));
}

void TRS_BatchAoSRenderer::submit(const Transform2D &transform) {
    batch.add(transform);
}

void TRS_BatchAoSRenderer::submitBatch(Transform2D::Container &transforms) {
    batch.append(transforms);
}

void TRS_BatchAoSRenderer::render(RenderMesh &renderMesh, RenderMaterial &renderMat) {
    buffer.bind();
    buffer.setData(batch.transforms.size() * sizeof(Transform2D), batch.transforms.data());

    renderMat.shaderProgram.use();
    renderMesh.layout.bind();

    renderMat.shaderProgram.setUniformMat4("projection", cachedCameraProjection);
    renderMat.shaderProgram.setUniform2f("camPos", cachedCameraPosition);

    int size = static_cast<int>(batch.transforms.size());
    glDrawElementsInstanced(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)0, size);

    batch.clear();
}
