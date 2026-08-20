#include "TRS_BatchSoARenderer.h"

#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "OrthoCamera.h"
#include "RenderObjects.h"

#include "profiling.h"

void TRS_BatchSoARenderer::initialize(const RenderMesh &renderMesh) {
    ZoneScoped;
    buffers.initialize(3);
    renderMesh.layout.bind();

    buffers.bind(0);
    renderMesh.layout.set(10, { GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(1);
    renderMesh.layout.set(11, { GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(2);
    renderMesh.layout.set(12, { GL_FLOAT, 0, 1, 1, 1 });
}

void TRS_BatchSoARenderer::submit(int matId, const Transform2D &transform) {
    ZoneScoped;
    batches[matId].add(transform);
}

void TRS_BatchSoARenderer::submitBatch(int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    batches[matId].append(transforms);
}

void TRS_BatchSoARenderer::render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects) {
    ZoneScoped;
    for (auto &[matId, batch] : batches) {
        if (batch.size() == 0) {
            continue;
        }

        buffers.bind(0);
        buffers.setData(batch.positions.size() * sizeof(glm::vec2), batch.positions.data());
        buffers.bind(1);
        buffers.setData(batch.scales.size() * sizeof(glm::vec2), batch.scales.data());
        buffers.bind(2);
        buffers.setData(batch.angles.size() * sizeof(float), batch.angles.data());

        auto &renderMat = renderObjects.getRenderMaterial(matId);

        renderMat.shaderProgram.use();
        renderMesh.layout.bind();

        renderMat.shaderProgram.setUniform2f("camPos", camera.position);
        renderMat.shaderProgram.setUniformMat4("projection", camera.projection);

        int size = static_cast<int>(batch.positions.size());
        glDrawElementsInstanced(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)0, size);

        batch.clear();
    }
}
