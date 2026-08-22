#include "Renderer.h"
#include "profiling.h"

#include "TRS_BatchSoARenderer.h"

void Renderer::initialize(RenderObjects &&renderObjects) {
    ZoneScoped;
    int meshCount = renderObjects.getRenderMeshSize();
    for (int i = 0; i < meshCount; ++i) {
        const RenderMesh &renderMesh = renderObjects.getRenderMesh(i);
        BatchRenderer &renderer = renderers.emplace_back();
        renderer.initialize(renderMesh);
        renderMesh.bindRendererLayout(renderer);
    }

    this->renderObjects = std::move(renderObjects);
}

void Renderer::submit(int meshId, int matId, const Transform2D &transform) {
    ZoneScoped;
    renderers[meshId].submit(matId, transform);
}

void Renderer::submitBatch(int meshId, int matId, const Transform2D::Container &transforms) {
    ZoneScoped;
    renderers[meshId].submitBatch(matId, transforms);
}

void Renderer::submitBatch(int meshId, int matId, const Transform2D::Container &transforms, int size) {
    ZoneScoped;
    renderers[meshId].submitBatch(matId, transforms, size);
}

void Renderer::render(OrthoCamera &camera) {
    ZoneScoped;
    for (int i = 0; i < (int)renderers.size(); ++i) {
        renderers[i].render(i, camera, renderObjects);
    }
}
