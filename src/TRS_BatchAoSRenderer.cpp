#include "TRS_BatchAoSRenderer.hpp"

#include "RenderMesh.hpp"
#include "RenderMaterial.hpp"
#include "OrthoCamera.hpp"
#include "RenderObjects.hpp"
#include "profiling.hpp"

void TRS_BatchAoSRenderer::initialize(const RenderMesh &renderMesh) {
    ZoneScoped;
    buffer.initialize();

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

void TRS_BatchAoSRenderer::bindLayout(const gl::BufferLayout &layout) const {
    layout.bind();

    buffer.bind();
    layout.set(gl::BufferLayout::Aggregate<Transform2D, float>(1), 10);
}

void TRS_BatchAoSRenderer::submit(int matId, const Transform2D &transform) {
    batches[matId].add(transform);
}

void TRS_BatchAoSRenderer::submitBatch(int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    batches[matId].append(transforms);
}

void TRS_BatchAoSRenderer::submitBatch(int matId, const Transform2D::Container &transforms, int size) {
    ZoneScoped;
    batches[matId].append(transforms, size);
}

void TRS_BatchAoSRenderer::render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects) {
    ZoneScoped;
    for (auto &[matId, batch] : batches) {
        buffer.bind();
        buffer.setData(batch.transforms.size() * sizeof(Transform2D), batch.transforms.data());

        auto &renderMat = renderObjects.getRenderMaterial(matId);
        renderMat.activate();
        renderMat.setShaderCameraState(camera);

        const RenderMesh &renderMesh = renderObjects.getRenderMesh(meshId);
        renderMesh.activate();

        renderCall(renderMesh, static_cast<int>(batch.size()));

        batch.clear();
    }
}
