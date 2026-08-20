#include "Renderer.h"
#include "profiling.h"

#include "TRS_BatchSoARenderer.h"

void Renderer::initialize(RenderObjects &&renderObjects) {
    ZoneScoped;
    int meshCount = renderObjects.getRenderMeshSize();
    for (int i = 0; i < meshCount; ++i) {
        renderers.emplace_back().initialize(renderObjects.getRenderMesh(i));
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

void Renderer::render(OrthoCamera &camera) {
    ZoneScoped;
    for (int i = 0; i < (int)renderers.size(); ++i) {
        auto &renderMesh = renderObjects.getRenderMesh(i);
        renderers[i].render(camera, renderMesh, renderObjects);
    }
}
