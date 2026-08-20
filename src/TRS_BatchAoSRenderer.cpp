#include "TRS_BatchAoSRenderer.h"

#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "OrthoCamera.h"
#include "RenderObjects.h"
#include "profiling.h"

void TRS_BatchAoSRenderer::initialize(const RenderMesh &renderMesh) {
    ZoneScoped;
    buffer.initialize();
    renderMesh.layout.bind();

    buffer.bind();

    renderMesh.layout.set(gl::BufferLayout::Aggregate<Transform2D, float>(1), 10);
}

void TRS_BatchAoSRenderer::submit(int matId, const Transform2D &transform) {
    ZoneScoped;
    batches[matId].add(transform);
}

void TRS_BatchAoSRenderer::submitBatch(int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    batches[matId].append(transforms);
}

void TRS_BatchAoSRenderer::render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects) {
    ZoneScoped;
    for (auto &[matId, batch] : batches) {
        buffer.bind();
        buffer.setData(batch.transforms.size() * sizeof(Transform2D), batch.transforms.data());

        auto &renderMat = renderObjects.getRenderMaterial(matId);

        renderMat.shaderProgram.use();
        renderMesh.layout.bind();

        renderMat.shaderProgram.setUniform2f("camPos", camera.position);
        renderMat.shaderProgram.setUniformMat4("projection", camera.projection);

        int size = static_cast<int>(batch.transforms.size());
        glDrawElementsInstanced(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)0, size);

        batch.clear();
    }
}
