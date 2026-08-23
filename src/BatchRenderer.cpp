#include "BatchRenderer.h"
#include "profiling.h"

#include "TRS_BatchSoARenderer.h"
#include "TRS_BatchAoSRenderer.h"

BatchRenderer::BatchRenderer() noexcept
    : TaggedPointer(new TRS_BatchSoARenderer())
{}

void BatchRenderer::initialize(const RenderMesh &renderMesh) {
    ZoneScoped;
    Dispatch([&renderMesh](auto *obj) {
            obj->initialize(renderMesh);
        });
}

void BatchRenderer::bindLayout(const gl::BufferLayout &layout) const {
    ZoneScoped;
    Dispatch([&layout](auto *obj) {
            obj->bindLayout(layout);
        });
}

void BatchRenderer::submit(int matId, const Transform2D &transform) {
    Dispatch([&matId, &transform](auto *obj) {
            obj->submit(matId, transform);
        });
}

void BatchRenderer::submitBatch(int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    Dispatch([&matId, &transforms](auto *obj) {
            obj->submitBatch(matId, transforms);
        });
}

void BatchRenderer::submitBatch(int matId, const Transform2D::Container &transforms, int size) {
    ZoneScoped;
    Dispatch([&matId, &transforms, &size](auto *obj) {
            obj->submitBatch(matId, transforms, size);
        });
}

void BatchRenderer::render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects) {
    ZoneScoped;
    Dispatch([&camera, &meshId, &renderObjects](auto *obj) {
            obj->render(meshId, camera, renderObjects);
        });
}
