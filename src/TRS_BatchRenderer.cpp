#include "TRS_BatchRenderer.h"
#include "profiling.h"

#include "TRS_BatchSoARenderer.h"
#include "TRS_BatchAoSRenderer.h"

void TRS_BatchRenderer::initialize() {
    ZoneScoped;
    Dispatch([](auto *obj) {
            obj->initialize();
        });
}

void TRS_BatchRenderer::bindRenderMesh(RenderMesh &renderMesh) {
    ZoneScoped;
    Dispatch([&renderMesh](auto *obj) {
            obj->bindRenderMesh(renderMesh);
        });
}

void TRS_BatchRenderer::submit(const Transform2D &transform) {
    ZoneScoped;
    Dispatch([&transform](auto *obj) {
            obj->submit(transform);
        });
}

void TRS_BatchRenderer::submitBatch(Transform2D::Container &transforms) {
    ZoneScoped;
    Dispatch([&transforms](auto *obj) {
            obj->submitBatch(transforms);
        });
}

void TRS_BatchRenderer::render(RenderMesh &renderMesh, RenderMaterial &renderMat) {
    ZoneScoped;
    Dispatch([&renderMesh, &renderMat](auto *obj) {
            obj->render(renderMesh, renderMat);
        });
}

void TRS_BatchRenderer::setCameraState(OrthoCamera &camera) {
    ZoneScoped;
    Dispatch([&camera](auto *obj) {
            obj->setCameraState(camera);
        });
}
