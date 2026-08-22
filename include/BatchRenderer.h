#pragma once

#include "TaggedPointer.h"
#include "Transform2D.h"

struct RenderMesh;
struct TRS_BatchSoARenderer;
struct TRS_BatchAoSRenderer;
struct OrthoCamera;
class RenderObjects;

struct BatchRenderer : TaggedPointer<TRS_BatchSoARenderer, TRS_BatchAoSRenderer> {
    using TaggedPointer::TaggedPointer;

    BatchRenderer() noexcept;
    void initialize(const RenderMesh &renderMesh);
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void submitBatch(int matId, const Transform2D::Container &transforms, int size);
    void render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects);

};
