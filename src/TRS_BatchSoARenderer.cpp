#include "TRS_BatchSoARenderer.h"

#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "OrthoCamera.h"
#include "RenderObjects.h"

#include "profiling.h"

void TRS_BatchSoARenderer::initialize(const RenderMesh &renderMesh) {
    ZoneScoped;
    buffers.initialize(3);

    if (renderMesh.getIndexCount() > 0) {
        renderCall = [](const RenderMesh &renderMesh, int size) {
            { ZoneScopedN("glDrawElementsInstanced");
            glDrawElementsInstanced(GL_TRIANGLE_FAN,
                    static_cast<int>(renderMesh.getIndexCount()), GL_UNSIGNED_INT, (void*)0, size);
            }
        };
    }
    else {
        renderCall = [](const RenderMesh &renderMesh, int size) {
            { ZoneScopedN("glDrawArraysInstanced");
            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0,
                    static_cast<int>(renderMesh.getVertexCount()), size);
            }
        };
    }

}

void TRS_BatchSoARenderer::bindLayout(const gl::BufferLayout &layout) const {
    layout.bind();

    buffers.bind(0);
    layout.set(10, { GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(1);
    layout.set(11, { GL_FLOAT, 0, 2, 2, 1 });

    buffers.bind(2);
    layout.set(12, { GL_FLOAT, 0, 1, 1, 1 });
}

void TRS_BatchSoARenderer::submit(int matId, const Transform2D &transform) {
    batches[matId].add(transform);
}

void TRS_BatchSoARenderer::submitBatch(int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    batches[matId].append(transforms);
}

void TRS_BatchSoARenderer::submitBatch(int matId, const Transform2D::Container &transforms, int size) {
    ZoneScoped;
    batches[matId].append(transforms, size);
}

void TRS_BatchSoARenderer::render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects) {
    ZoneScoped;

    for (auto &[matId, batch] : batches) {
        if (batch.size() == 0) {
            continue;
        }

        buffers.bind(0);
        buffers.setData(batch.size() * sizeof(glm::vec2), batch.positions.data());
        buffers.bind(1);
        buffers.setData(batch.size() * sizeof(glm::vec2), batch.scales.data());
        buffers.bind(2);
        buffers.setData(batch.size() * sizeof(float), batch.angles.data());

        auto &renderMat = renderObjects.getRenderMaterial(matId);
        renderMat.activate();
        renderMat.setShaderCameraState(camera);

        const RenderMesh &renderMesh = renderObjects.getRenderMesh(meshId);
        renderMesh.activate();

        renderCall(renderMesh, static_cast<int>(batch.size()));

        batch.clear();
    }
}
