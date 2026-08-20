#pragma once

#include "RenderMesh.h"
#include "Transform2D.h"
#include "gl/Buffers.h"

#include <unordered_map>

struct OrthoCamrea;
class RenderObjects;

struct TRS_BatchSoARenderer {
    std::unordered_map<int, Transform2D::SoA> batches;
    gl::Buffers buffers;

    TRS_BatchSoARenderer() noexcept
        : buffers(GL_ARRAY_BUFFER)
    {}

    void initialize(const RenderMesh &renderMesh);
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects);
};

