#pragma once

#include "TaggedPointer.hpp"
#include "Transform2D.hpp"
#include "gl/BufferLayout.hpp"

struct TRS_BatchSoARenderer;
struct TRS_BatchAoSRenderer;
struct OrthoCamera;

class RenderMesh;
class RenderObjects;

struct BatchRenderer : TaggedPointer<TRS_BatchSoARenderer, TRS_BatchAoSRenderer> {
    using TaggedPointer::TaggedPointer;

    BatchRenderer() noexcept;
    void initialize(const RenderMesh &renderMesh);
    void bindLayout(const gl::BufferLayout &layout) const;
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void submitBatch(int matId, const Transform2D::Container &transforms, int size);
    void render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects);

};
