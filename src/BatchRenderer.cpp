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

void BatchRenderer::submit(int matId, const Transform2D &transform) {
    ZoneScoped;
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

void BatchRenderer::render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects) {
    ZoneScoped;
    Dispatch([&camera, &renderMesh, &renderObjects](auto *obj) {
            obj->render(camera, renderMesh, renderObjects);
        });
}
